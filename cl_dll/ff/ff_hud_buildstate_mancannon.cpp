#include "cbase.h"
#include "ff_hud_buildstate_mancannon.h"

DECLARE_HUDELEMENT(CHudBuildStateManCannon);
DECLARE_HUD_MESSAGE(CHudBuildStateManCannon, ManCannonMsg);

CHudBuildStateManCannon::CHudBuildStateManCannon(const char *pElementName) : CHudElement(pElementName), BaseClass(NULL, "HudBuildStateManCannon")
{
	SetParent(g_pClientMode->GetViewport());

	// Hide when player is dead and not scout
	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NOTSCOUT );

	SetUseToggleText(true);

	m_bBuilt = false;
	m_bBuilding = false;
}
	
CHudBuildStateManCannon::~CHudBuildStateManCannon() 
{
}

KeyValues* CHudBuildStateManCannon::GetDefaultStyleData()
{
	KeyValues *kvPreset = new KeyValues("StyleData");

	kvPreset->SetInt("x", 480);
	kvPreset->SetInt("y", 330);
	kvPreset->SetInt("alignH", 0);
	kvPreset->SetInt("alignV", 0);

	kvPreset->SetInt("columns", 1);
	kvPreset->SetInt("headerTextX", 27);
	kvPreset->SetInt("headerTextY", 8);
	kvPreset->SetInt("headerIconX", 5);
	kvPreset->SetInt("headerIconY", 3);
	kvPreset->SetInt("textX", 25);
	kvPreset->SetInt("textY", 22);
	kvPreset->SetInt("itemsX", 5);
	kvPreset->SetInt("itemsY", 22);
	kvPreset->SetInt("showHeaderText", 1);
	kvPreset->SetInt("showHeaderIcon", 1);
	kvPreset->SetInt("showText", 1);
	kvPreset->SetInt("headerTextShadow", 0);
	kvPreset->SetInt("headerIconShadow", 0);
	kvPreset->SetInt("textShadow", 0);
	kvPreset->SetInt("headerTextSize", 2);
	kvPreset->SetInt("headerIconSize", 6);
	kvPreset->SetInt("textSize", 3);
	kvPreset->SetInt("showPanel", 1);
	kvPreset->SetInt("panelColorCustom", 0);
	kvPreset->SetInt("panelRed", 255);
	kvPreset->SetInt("panelGreen", 255);
	kvPreset->SetInt("panelBlue", 255);
	kvPreset->SetInt("panelAlpha", 255);

	kvPreset->SetInt("barWidth", 60);
	kvPreset->SetInt("barHeight", 10);
	kvPreset->SetInt("barBorderWidth", 1);
	kvPreset->SetInt("barMarginHorizontal", 0);
	kvPreset->SetInt("barMarginVertical", 0);
	kvPreset->SetInt("barOrientation", ORIENTATION_HORIZONTAL);

	KeyValues *kvComponent = new KeyValues("Bar");
	kvComponent->SetInt("show", 1);
	kvComponent->SetInt("colorMode", COLOR_MODE_FADED);
	kvComponent->SetInt("red", 255);
	kvComponent->SetInt("green", 255);
	kvComponent->SetInt("blue", 255);
	kvComponent->SetInt("alpha", 93);

	kvPreset->AddSubKey(kvComponent);

	kvComponent = new KeyValues("BarBorder");
	kvComponent->SetInt("show", 1);
	kvComponent->SetInt("colorMode", COLOR_MODE_CUSTOM);
	kvComponent->SetInt("red", 255);
	kvComponent->SetInt("green", 255);
	kvComponent->SetInt("blue", 255);
	kvComponent->SetInt("alpha", 155);

	kvPreset->AddSubKey(kvComponent);

	kvComponent = new KeyValues("BarBackground");
	kvComponent->SetInt("show", 1);
	kvComponent->SetInt("colorMode", COLOR_MODE_FADED);
	kvComponent->SetInt("red", 255);
	kvComponent->SetInt("green", 255);
	kvComponent->SetInt("blue", 255);
	kvComponent->SetInt("alpha", 65);

	kvPreset->AddSubKey(kvComponent);

	kvComponent = new KeyValues("Icon");
	kvComponent->SetInt("show", 0);
	kvComponent->SetInt("colorMode", COLOR_MODE_CUSTOM);
	kvComponent->SetInt("red", 255);
	kvComponent->SetInt("green", 255);
	kvComponent->SetInt("blue", 255);
	kvComponent->SetInt("alpha", 255);
	kvComponent->SetInt("shadow", 0);
	kvComponent->SetInt("size", 2);
	kvComponent->SetInt("alignH", ALIGN_CENTER);
	kvComponent->SetInt("alignV", ALIGN_MIDDLE);
	kvComponent->SetInt("offsetX", 5);
	kvComponent->SetInt("offsetY", 0);

	kvPreset->AddSubKey(kvComponent);

	kvComponent = new KeyValues("Label");
	kvComponent->SetInt("show", 1);
	kvComponent->SetInt("colorMode", COLOR_MODE_CUSTOM);
	kvComponent->SetInt("red", 255);
	kvComponent->SetInt("green", 255);
	kvComponent->SetInt("blue", 255);
	kvComponent->SetInt("alpha", 255);
	kvComponent->SetInt("shadow", 1);
	kvComponent->SetInt("size", 1);
	kvComponent->SetInt("alignH", ALIGN_LEFT);
	kvComponent->SetInt("alignV", ALIGN_MIDDLE);
	kvComponent->SetInt("offsetX", 54);
	kvComponent->SetInt("offsetY", 0);
	kvComponent->SetInt("fontTahoma", 1);

	kvPreset->AddSubKey(kvComponent);

	kvComponent = new KeyValues("Amount");
	kvComponent->SetInt("show", 1);
	kvComponent->SetInt("colorMode", COLOR_MODE_FADED);
	kvComponent->SetInt("red", 255);
	kvComponent->SetInt("green", 255);
	kvComponent->SetInt("blue", 255);
	kvComponent->SetInt("alpha", 255);
	kvComponent->SetInt("shadow", 0);
	kvComponent->SetInt("size", 0);
	kvComponent->SetInt("alignH", ALIGN_RIGHT);
	kvComponent->SetInt("alignV", ALIGN_CENTER);
	kvComponent->SetInt("offsetX", 3);
	kvComponent->SetInt("offsetY", 1);
	kvComponent->SetInt("fontTahoma", 0);

	kvPreset->AddSubKey(kvComponent);

	return kvPreset;
}
	
void CHudBuildStateManCannon::VidInit()
{
	wchar_t *tempString = vgui::localize()->Find("#FF_PLAYER_MANCANNON");

	if (!tempString) 
		tempString = L"MANCANNON";

	SetHeaderText(tempString, false);
	SetHeaderIconChar("6", false);

	m_wszNotBuiltText = vgui::localize()->Find("#HudPanel_NotBuilt"); 

	if (!m_wszNotBuiltText) 
		m_wszNotBuiltText = L"Not Built";
	SetText(m_wszNotBuiltText, false);

	m_wszBuildingText = vgui::localize()->Find("#HudPanel_Building"); 

	if (!m_wszBuildingText) 
		m_wszBuildingText = L"Building...";

	m_qbManCannonHealth->SetLabelText("#FF_ITEM_HEALTH", false);
	m_qbManCannonHealth->SetIconChar("a", false);
	m_qbManCannonHealth->ShowAmountMax(false);
	m_qbManCannonHealth->SetAmount(0);
	m_qbManCannonHealth->SetVisible(false);

	SetToggleTextVisible(true);
}

void CHudBuildStateManCannon::Init() 
{
	ivgui()->AddTickSignal(GetVPanel(), 250); //only update 4 times a second
	HOOK_HUD_MESSAGE(CHudBuildStateManCannon, ManCannonMsg);

	m_qbManCannonHealth = AddItem("BuildStateManCannonHealth"); 

	AddPanelToHudOptions("ManCannon", "#HudPanel_ManCannon", "BuildState", "#HudPanel_BuildableState");
}

void CHudBuildStateManCannon::OnTick() 
{
	BaseClass::OnTick();

	if( !engine->IsInGame() | !ShouldDraw() )
		return;

	// Get the local player
	C_FFPlayer *pPlayer = C_FFPlayer::GetLocalFFPlayer();

	bool bBuilt = pPlayer->GetManCannon() && pPlayer->GetManCannon()->IsBuilt();
	bool bBuilding = pPlayer->GetManCannon() && !bBuilt;

	//small optimisation by comparing building with what it was previously
	//if building
	if(bBuilding && !m_bBuilding)
	//show building text
	{
		SetText(m_wszBuildingText);
		m_bBuilding = bBuilding;
	}
	//if not building
	else if(!bBuilding && m_bBuilding)
	//show not built text
	{
		SetText(m_wszNotBuiltText);
		m_bBuilding = bBuilding;
	}

	//small optimisation by comparing build with what it was previously
	//if not built
	if(!bBuilt && m_bBuilt)
	//hide quantity bars
	{
		m_bBuilt = false;
		m_qbManCannonHealth->SetVisible(false);
		SetToggleTextVisible(true);
	}
	else if(bBuilt && !m_bBuilt)
	//show quantity bars
	{
		m_bBuilt = true;
		m_qbManCannonHealth->SetVisible(true);
		SetToggleTextVisible(false);
	}
}

void CHudBuildStateManCannon::Paint() 
{
	//paint header
	BaseClass::Paint();
}

void CHudBuildStateManCannon::MsgFunc_ManCannonMsg(bf_read &msg)
{
    int iHealth = (int) msg.ReadByte();

	m_qbManCannonHealth->SetAmount(iHealth);
}