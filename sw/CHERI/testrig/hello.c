#include <cheri_init_globals.h>
#include <cheric.h>

void init_globals(){
	cheri_init_globals_3(__builtin_cheri_global_data_get(),
 	__builtin_cheri_program_counter_get(),
 	__builtin_cheri_global_data_get());
}

int main() {
	//while (1)
	return (0);
}


