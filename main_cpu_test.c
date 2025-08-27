#include "emulator.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <dirent.h>

static void initialize_cpu(cJSON* initial)
{
	cJSON* item;
	
	item = cJSON_GetObjectItem(initial, "pc");
	cpu.reg16_PC = (u16)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "sp");
	cpu.reg16_SP = (u16)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "a");
	*cpu.reg8_A = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "f");
	*cpu.reg8_F = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "b");
	*cpu.reg8_B = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "c");
	*cpu.reg8_C = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "d");
	*cpu.reg8_D = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "e");
	*cpu.reg8_E = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "h");
	*cpu.reg8_H = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "l");
	*cpu.reg8_L = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "ime");
	cpu.IME = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "ie");
	cpu.IME_set_request = (u8)cJSON_GetNumberValue(item);

	item = cJSON_GetObjectItem(initial, "ram");
	for (size_t i = 0; i < cJSON_GetArraySize(item); i++)
	{
		cJSON* kv_pair = cJSON_GetArrayItem(item, i);
		u16 addr = (u16)cJSON_GetNumberValue(cJSON_GetArrayItem(kv_pair, 0));
		u8 val = (u8)cJSON_GetNumberValue(cJSON_GetArrayItem(kv_pair, 1));
		write8(addr, val);
	}
}

static int validate_cpu(cJSON* final)
{
	cJSON* item;

	item = cJSON_GetObjectItem(final, "pc");
	if (item && cpu.reg16_PC != (u16)cJSON_GetNumberValue(item)) {
		printf("Mismatch: PC expected %u got %u\n", (u16)cJSON_GetNumberValue(item), cpu.reg16_PC);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "sp");
	if (item && cpu.reg16_SP != (u16)cJSON_GetNumberValue(item)) {
		printf("Mismatch: SP expected %u got %u\n", (u16)cJSON_GetNumberValue(item), cpu.reg16_SP);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "a");
	if (item && *cpu.reg8_A != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: A expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_A);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "f");
	if (item && *cpu.reg8_F != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: F expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_F);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "b");
	if (item && *cpu.reg8_B != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: B expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_B);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "c");
	if (item && *cpu.reg8_C != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: C expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_C);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "d");
	if (item && *cpu.reg8_D != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: D expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_D);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "e");
	if (item && *cpu.reg8_E != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: E expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_E);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "h");
	if (item && *cpu.reg8_H != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: H expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_H);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "l");
	if (item && *cpu.reg8_L != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: L expected %u got %u\n", (u8)cJSON_GetNumberValue(item), *cpu.reg8_L);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "ime");
	if (item && cpu.IME != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: IME expected %u got %u\n", (u8)cJSON_GetNumberValue(item), cpu.IME);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "ie");
	if (item && (cpu.IME_set_request > 0) != (u8)cJSON_GetNumberValue(item)) {
		printf("Mismatch: IE expected %u got %u\n", (u8)cJSON_GetNumberValue(item), cpu.IME_set_request);
		return 0;
	}

	item = cJSON_GetObjectItem(final, "ram");
	for (size_t i = 0; i < cJSON_GetArraySize(item); i++)
	{
		cJSON* kv_pair = cJSON_GetArrayItem(item, i);
		u16 addr = (u16)cJSON_GetNumberValue(cJSON_GetArrayItem(kv_pair, 0));
		u8 val = (u8)cJSON_GetNumberValue(cJSON_GetArrayItem(kv_pair, 1));
		u8 got = read8(addr);
		if (got != val) {
			printf("Mismatch: RAM[%u] expected %u got %u\n", addr, val, got);
			return 0;
		}
	}

	return 1;
}

static void run_json_test(cJSON *test)
{
	// set initial cpu and ram state
	memory = (memory_t){0};
	if (!init_timers())
		return;
	if (!init_cpu())
		return;

	cJSON *name = cJSON_GetObjectItem(test, "name");
	initialize_cpu(cJSON_GetObjectItem(test, "initial"));

	do
	{
		cpu_tick();
	} while (cpu.instruction_cycles_remain > 0 || cpu.prefix_instruction);

	// compare to final state
	if (!validate_cpu(cJSON_GetObjectItem(test, "final")))
	{
		printf("Name: %s\n", name->valuestring);
		cpu_log();
		exit(0);
	}
}

static void run_json_test_file(char* path)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		printf("Error: Unable to open %s.\n", path);
		return;
	}
	printf("path: %s.\n", path);

	fseek(fp, 0l, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	char* buffer = calloc(len, 1);
	fread(buffer, 1, len, fp);
	fclose(fp);

	// parse the JSON data
	cJSON *json = cJSON_Parse(buffer);
	free(buffer);
	if (json == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
			printf("Error: %s\n", error_ptr);
		cJSON_Delete(json);
		return;
	}

	// for each test...
	for (size_t i = 0; i < cJSON_GetArraySize(json); i++)
	{
		cJSON *test = cJSON_GetArrayItem(json, i);
		run_json_test(test);
	}
	
	cJSON_Delete(json);
}

static void run_json_test_file_index(char* path, int index)
{
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		printf("Error: Unable to open %s.\n", path);
		return;
	}
	printf("path: %s.\n", path);

	fseek(fp, 0l, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	char* buffer = calloc(len, 1);
	fread(buffer, 1, len, fp);
	fclose(fp);

	// parse the JSON data
	cJSON *json = cJSON_Parse(buffer);
	free(buffer);
	if (json == NULL)
	{
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL)
			printf("Error: %s\n", error_ptr);
		cJSON_Delete(json);
		return;
	}

	cJSON *test = cJSON_GetArrayItem(json, index);
	run_json_test(test);
	
	cJSON_Delete(json);
}

static void run_all_json_test_files()
{
	struct dirent *de;
	DIR *dr = opendir("json tests");
	if (dr == NULL)
	{
		printf("Could not open current directory" );
		return;
	}
	// for each json file...
	while ((de = readdir(dr)) != NULL)
	{
		if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, "..") || de->d_name[0] == '!')
			continue;

		char path[1024];
		sprintf(path, "%s/%s", "json tests", de->d_name);
		run_json_test_file(path);
	}
	free(dr);
}

// int main()
// {
// 	run_all_json_test_files();
// 	// run_json_test_file("json tests/fb.json");
// 	// run_json_test_file_index("json tests/cb ea.json", 0x0211);
// 	return 0;
// }
