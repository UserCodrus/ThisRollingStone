#include "MenuWidget.h"

/// Object Interface ///

void UMenuWidget::SetOwner(AMainMenuHUD* NewParent)
{
	if (Owner == nullptr)
	{
		Owner = NewParent;
	}
}