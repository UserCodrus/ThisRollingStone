#include "MainMenuMode.h"
#include "BaseController.h"

AMainMenuMode::AMainMenuMode()
{
	// Set default classes
	PlayerControllerClass = ABaseController::StaticClass();
}