// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.


// IMPORTANT: UI etc should modify g_Config. Graphics code should read g_ActiveConfig.
// The reason for this is to get rid of race conditions etc when the configuration
// changes in the middle of a frame. This is done by copying g_Config to g_ActiveConfig
// at the start of every frame. Noone should ever change members of g_ActiveConfig
// directly.

#pragma once

#include <string>
#include <vector>

#include "Common/CommonTypes.h"
#include "VideoCommon/VideoCommon.h"

// Log in two categories, and save three other options in the same byte
#define CONF_LOG          1
#define CONF_PRIMLOG      2
#define CONF_SAVETARGETS  8
#define CONF_SAVESHADERS  16

enum AspectMode
{
	ASPECT_AUTO       = 0,
	ASPECT_FORCE_16_9 = 1,
	ASPECT_FORCE_4_3  = 2,
	ASPECT_STRETCH    = 3,
};

enum EFBScale
{
	SCALE_FORCE_INTEGRAL = -1,
	SCALE_AUTO,
	SCALE_AUTO_INTEGRAL,
	SCALE_1X,
	SCALE_1_5X,
	SCALE_2X,
	SCALE_2_5X,
	SCALE_3X,
	SCALE_4X,
};

// NEVER inherit from this class.
struct VideoConfig final
{
	VideoConfig();
	void Load(const std::string& ini_file);
	void LoadVR(const std::string& ini_file);
	void GameIniLoad();
	void GameIniSave();
	void VerifyValidity();
	void Save(const std::string& ini_file);
	void SaveVR(const std::string& ini_file);
	void UpdateProjectionHack();
	bool IsVSync();
	bool VRSettingsModified();

	// General
	bool bVSync;
	bool bFullscreen;
	bool bRunning;
	bool bWidescreenHack;
	int iAspectRatio;
	bool bCrop;   // Aspect ratio controls.
	bool bUseXFB;
	bool bUseRealXFB;

	// Enhancements
	int iMultisampleMode;
	int iEFBScale;
	bool bForceFiltering;
	int iMaxAnisotropy;
	std::string sPostProcessingShader;

	// Information
	bool bShowFPS;
	bool bOverlayStats;
	bool bOverlayProjStats;
	bool bTexFmtOverlayEnable;
	bool bTexFmtOverlayCenter;
	bool bShowEFBCopyRegions;
	bool bLogRenderTimeToFile;

	// Render
	bool bWireFrame;
	bool bDstAlphaPass;
	bool bDisableFog;

	// Utility
	bool bDumpTextures;
	bool bHiresTextures;
	bool bDumpEFBTarget;
	bool bUseFFV1;
	bool bFreeLook;
	bool bAnaglyphStereo;
	int iAnaglyphStereoSeparation;
	int iAnaglyphFocalAngle;
	bool bBorderlessFullscreen;

	// Hacks
	bool bEFBAccessEnable;
	bool bPerfQueriesEnable;

	bool bEFBCopyEnable;
	bool bEFBCopyClearDisable;
	bool bEFBCopyCacheEnable;
	bool bEFBEmulateFormatChanges;
	bool bCopyEFBToTexture;
	bool bCopyEFBScaled;
	int iSafeTextureCache_ColorSamples;
	int iPhackvalue[3];
	std::string sPhackvalue[2];
	float fAspectRatioHackW, fAspectRatioHackH;
	bool bEnablePixelLighting;
	bool bFastDepthCalc;
	int iLog; // CONF_ bits
	int iSaveTargetId; // TODO: Should be dropped

	// VR global
	float fScale;
	float fLeanBackAngle;
	bool bAsynchronousTimewarp;
	bool bEnableVR;
	bool bLowPersistence;
	bool bDynamicPrediction;
	bool bOrientationTracking;
	bool bMagYawCorrection;
	bool bPositionTracking;
	bool bChromatic;
	bool bTimewarp;
	bool bVignette;
	bool bNoRestore;
	bool bFlipVertical;
	bool bSRGB;
	bool bOverdrive;
	bool bHqDistortion;
	int iVRPlayer;
	u32 iMinExtraFrames;
	u32 iMaxExtraFrames;

	// VR
	float fUnitsPerMetre;
	float fHudThickness;
	float fHudDistance;
	float fHud3DCloser;
	float fCameraForward;
	float fCameraPitch;
	float fAimDistance;
	float fScreenHeight;
	float fScreenThickness;
	float fScreenDistance;
	float fScreenRight;
	float fScreenUp;
	float fScreenPitch;
	float fTelescopeMaxFOV;
	bool bDisable3D;
	bool bHudFullscreen;
	bool bHudOnTop;
	int iTelescopeEye;
	int iMetroidPrime;
	// VR layer debugging
	int iSelectedLayer;
	int iFlashState;

	// D3D only config, mostly to be merged into the above
	int iAdapter;

	// Debugging
	bool bEnableShaderDebugging;

	// Static config per API
	// TODO: Move this out of VideoConfig
	struct
	{
		API_TYPE APIType;

		std::vector<std::string> Adapters; // for D3D
		std::vector<std::string> AAModes;
		std::vector<std::string> PPShaders; // post-processing shaders

		bool bUseMinimalMipCount;
		bool bSupportsExclusiveFullscreen;
		bool bSupportsDualSourceBlend;
		bool bSupportsPrimitiveRestart;
		bool bSupportsOversizedViewports;
		bool bSupportsEarlyZ; // needed by PixelShaderGen, so must stay in VideoCommon
		bool bSupportsBindingLayout; // Needed by ShaderGen, so must stay in VideoCommon
		bool bSupportsBBox;
	} backend_info;

	// Utility
	bool RealXFBEnabled() const { return bUseXFB && bUseRealXFB; }
	bool VirtualXFBEnabled() const { return bUseXFB && !bUseRealXFB; }
	bool EFBCopiesToTextureEnabled() const { return bEFBCopyEnable && bCopyEFBToTexture; }
	bool EFBCopiesToRamEnabled() const { return bEFBCopyEnable && !bCopyEFBToTexture; }
	bool ExclusiveFullscreenEnabled() const { return backend_info.bSupportsExclusiveFullscreen && !bBorderlessFullscreen; }
};

extern VideoConfig g_Config;
extern VideoConfig g_ActiveConfig;

// Called every frame.
void UpdateActiveConfig();
