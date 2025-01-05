#include "UIModeController.h"

#include "utils/StringUtil.h"
#include "views/ViewController.h"
#include "Log.h"
#include "Window.h"

#define fake_gettext_full _("Full")
#define fake_gettext_kiosk _("Kiosk")
#define fake_gettext_kid _("Kid")
#define fake_gettext_basic _("Basic")

UIModeController *  UIModeController::sInstance = NULL;

UIModeController * UIModeController::getInstance()
{
	if (sInstance == NULL)
		sInstance = new UIModeController();

	return sInstance;
}

UIModeController::UIModeController()
	: mPassKeyCounter(0), mUIModeChanged(false)
{
	mPassKeySequence = Settings::getInstance()->getString("UIMode_passkey");
	mCurrentUIMode = Settings::getInstance()->getString("UIMode");

	Settings::settingChanged += this;
}

void UIModeController::onSettingChanged(const std::string& name)
{
	if (name == "UIMode")
	{
		mUIModeChanged = true;
		monitorUIMode();
	}
}

void UIModeController::monitorUIMode()
{
	if (!mUIModeChanged)
		return;

	mUIModeChanged = false;
	std::string uimode = Settings::getInstance()->getString("UIMode");
	if (uimode != mCurrentUIMode) // UIMODE HAS CHANGED
	{
		mCurrentUIMode = uimode;
		ViewController::get()->ReloadAndGoToStart();
	}
}

bool UIModeController::listen(InputConfig * config, Input input)
{
	// Reads the current input to listen for the passkey
	// sequence to unlock the UI mode. The progress is saved in mPassKeyCounter
	if (Settings::getInstance()->getBool("Debug"))
	{
		logInput(config, input);
	}

	if ((Settings::getInstance()->getString("UIMode") == "Full") || !isValidInput(config, input))
	{
		return false; // Already unlocked, or invalid input, nothing to do here.
	}


	if (!inputIsMatch(config, input))
	{
		mPassKeyCounter = 0; // current input is incorrect, reset counter
	}

	if (mPassKeyCounter == (int)mPassKeySequence.length())
	{
		unlockUIMode();
		return true;
	}
	return false;
}

bool UIModeController::inputIsMatch(InputConfig * config, Input input)
{
	for (auto valstring : mInputVals)
	{
		if (config->isMappedLike(valstring, input) &&
			(mPassKeySequence[mPassKeyCounter] == valstring[0]))
		{
			mPassKeyCounter++;
			return true;
		}
	}
	return false;
}

// When we have reached the end of the list, trigger UI_mode unlock
void UIModeController::unlockUIMode()
{
	LOG(LogDebug) << " UIModeController::listen(): Passkey sequence completed, switching UIMode to full";

	Settings::getInstance()->setString("UIMode", "Full");
	Settings::getInstance()->saveFile();
	mPassKeyCounter = 0;
}

bool UIModeController::LoadEmptySystems()
{
	return getInstance()->isUIModeFull() && Settings::LoadEmptySystems();
}

bool UIModeController::isUIModeFull()
{
	return (mCurrentUIMode == "Full" && !Settings::getInstance()->getBool("ForceKiosk") && !Settings::getInstance()->getBool("ForceKid"));
}

bool UIModeController::isUIModeBasic()
{
	return (mCurrentUIMode == "Basic" && !Settings::getInstance()->getBool("ForceKiosk") && !Settings::getInstance()->getBool("ForceKid"));
}

bool UIModeController::isUIModeKid()
{
	return (Settings::getInstance()->getBool("ForceKid") || (mCurrentUIMode == "Kid" && !Settings::getInstance()->getBool("ForceKiosk")));
}

bool UIModeController::isUIModeKiosk()
{
	return (Settings::getInstance()->getBool("ForceKiosk") || (mCurrentUIMode == "Kiosk" && !Settings::getInstance()->getBool("ForceKid")));
}

std::string UIModeController::getFormattedPassKeyStr()
{
	// supported sequence-inputs: u (up), d (down), l (left), r (right), a, b, x, y

	std::string out = "";
	for (auto c : mPassKeySequence)
	{
		out += (out == "") ? "" : ", ";  // add a comma after the first entry

		switch (c)
		{
		case 'u':
			out += Utils::String::unicode2Chars(0x2191); // arrow pointing up
			break;
		case 'd':
			out += Utils::String::unicode2Chars(0x2193); // arrow pointing down
			break;
		case 'l':
			out += Utils::String::unicode2Chars(0x2190); // arrow pointing left
			break;
		case 'r':
			out += Utils::String::unicode2Chars(0x2192); // arrow pointing right
			break;
		case 'a':
			out += "A";
			break;
		case 'b':
			out += "B";
			break;
		case 'x':
			out += "X";
			break;
		case 'y':
			out += "Y";
			break;
		}
	}
	return out;
}


void UIModeController::logInput(InputConfig * config, Input input)
{
	std::string mapname = "";
	std::vector<std::string> maps = config->getMappedTo(input);
	for( auto mn : maps)
	{
		mapname += mn;
		mapname += ", ";
	}
	LOG(LogDebug) << "UIModeController::logInput( " << config->getDeviceName() <<" ):" << input.string() << ", isMappedTo= " << mapname << ", value=" << input.value;
}

bool UIModeController::isValidInput(InputConfig * config, Input input)
{
	if((config->getMappedTo(input).size() == 0)  || // not a mapped input, so ignore.
		(input.type == TYPE_HAT) ||  // ignore all HAT inputs
		(!input.value))	// not a key-down event
	{
		return false;
	}
	else
	{
		return true;
	}
}