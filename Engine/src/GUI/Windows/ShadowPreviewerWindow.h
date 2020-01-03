#pragma once
#include "BaseWindow.h"

class ShadowPreviewerWindow final : public BaseWindow
{
protected:
	void Draw() override;

private:
	void DrawSettings();
	void DrawPreview() const;
	void DrawWarning() const;

	float ImageScale = 0.5f;
	float Bias = 2.0f;
};
