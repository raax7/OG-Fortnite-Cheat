#include "../Hooks.h"

bool Hooks::ShouldReplicateFunction::ShouldReplicateFunction(void* this_, SDK::AActor* Actor, void* Function) {
	//We return true in order to bypass the Games check of whether or not this Function should be proccesed by the server.
	//We hook this in order to call Functions normally not handled by the server with Invalid Paramaters in order to crash Server-Sided
	//Only hooked when the Server Crash button is pressed meaning it's safe to always return true
	return true;
}