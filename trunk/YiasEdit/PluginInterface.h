#ifndef _PLUGIN_INTERFACE_H
#define _PLUGIN_INTERFACE_H

typedef unsigned __int64 YEID;

typedef struct
{
	std::string pluginName;
	std::string pluginAutor;
	unsigned int versionMajor;
	unsigned int versionMinor;
} PLUGININFO, *LPPLUGININFO;

enum Hooks
{
	HOOK_SELECTION_CHANGED		= 0x00,
	HOOK_DRAW_3D				= 0x01,
	HOOK_DRAW_2D				= 0x02,

	NUM_HOOKS,
};

class CPluginManager abstract
{
public:
	virtual void SayHello() = 0;
	virtual void RegisterHook(Hooks hook, void* proc) = 0;
	virtual std::string GetModelName(YEID mid) = 0;
	virtual YEID AddLabel(unsigned int x, unsigned int y, const char* text = "") = 0;
	virtual void SetLabelText(YEID label, const char* text) = 0;
	virtual YEID GetSelection() = 0;
	virtual void RotateModel(YEID m, float x, float y, float z) = 0;
	virtual YEID AddButton(unsigned int x, unsigned int y, unsigned int w, unsigned int h, const char* text = "") = 0;
	virtual void AddButtonCallback(YEID m, void (*proc)(YEID)) = 0;
	virtual void Hide(YEID m) = 0;
	virtual void Show(YEID m) = 0;
};

#endif