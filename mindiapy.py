# This file was created automatically by SWIG.
import mindiapyc
class IColor:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,IColor):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = IColor.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = IColor.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(mindiapyc.new_IColor,args)
        self.thisown = 1
    __setmethods__["iRed"] = mindiapyc.IColor_iRed_set
    __getmethods__["iRed"] = mindiapyc.IColor_iRed_get
    __setmethods__["iGreen"] = mindiapyc.IColor_iGreen_set
    __getmethods__["iGreen"] = mindiapyc.IColor_iGreen_get
    __setmethods__["iBlue"] = mindiapyc.IColor_iBlue_set
    __getmethods__["iBlue"] = mindiapyc.IColor_iBlue_get
    def __del__(self,mindiapyc=mindiapyc):
        if getattr(self,'thisown',0):
            mindiapyc.delete_IColor(self)
    def __repr__(self):
        return "<C IColor instance at %s>" % (self.this,)

class IColorPtr(IColor):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = IColor
mindiapyc.IColor_swigregister(IColorPtr)
class IHsv:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,IHsv):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = IHsv.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = IHsv.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __init__(self,*args):
        self.this = apply(mindiapyc.new_IHsv,args)
        self.thisown = 1
    __setmethods__["iHue"] = mindiapyc.IHsv_iHue_set
    __getmethods__["iHue"] = mindiapyc.IHsv_iHue_get
    __setmethods__["iSaturation"] = mindiapyc.IHsv_iSaturation_set
    __getmethods__["iSaturation"] = mindiapyc.IHsv_iSaturation_get
    __setmethods__["iValue"] = mindiapyc.IHsv_iValue_set
    __getmethods__["iValue"] = mindiapyc.IHsv_iValue_get
    def __del__(self,mindiapyc=mindiapyc):
        if getattr(self,'thisown',0):
            mindiapyc.delete_IHsv(self)
    def __repr__(self):
        return "<C IHsv instance at %s>" % (self.this,)

class IHsvPtr(IHsv):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = IHsv
mindiapyc.IHsv_swigregister(IHsvPtr)
class IDiaOutputWindow:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,IDiaOutputWindow):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = IDiaOutputWindow.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = IDiaOutputWindow.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __del__(self,mindiapyc=mindiapyc):
        if getattr(self,'thisown',0):
            mindiapyc.delete_IDiaOutputWindow(self)
    def IsValid(*args): return apply(mindiapyc.IDiaOutputWindow_IsValid,args)
    def Show(*args): return apply(mindiapyc.IDiaOutputWindow_Show,args)
    def Hide(*args): return apply(mindiapyc.IDiaOutputWindow_Hide,args)
    def Maximize(*args): return apply(mindiapyc.IDiaOutputWindow_Maximize,args)
    def Restore(*args): return apply(mindiapyc.IDiaOutputWindow_Restore,args)
    def GetWidth(*args): return apply(mindiapyc.IDiaOutputWindow_GetWidth,args)
    def GetHeight(*args): return apply(mindiapyc.IDiaOutputWindow_GetHeight,args)
    def SetSize(*args): return apply(mindiapyc.IDiaOutputWindow_SetSize,args)
    def SetPos(*args): return apply(mindiapyc.IDiaOutputWindow_SetPos,args)
    def GetDrawWidth(*args): return apply(mindiapyc.IDiaOutputWindow_GetDrawWidth,args)
    def GetDrawHeight(*args): return apply(mindiapyc.IDiaOutputWindow_GetDrawHeight,args)
    def Clear(*args): return apply(mindiapyc.IDiaOutputWindow_Clear,args)
    def SetColor(*args): return apply(mindiapyc.IDiaOutputWindow_SetColor,args)
    def SetFont(*args): return apply(mindiapyc.IDiaOutputWindow_SetFont,args)
    def SetFontSize(*args): return apply(mindiapyc.IDiaOutputWindow_SetFontSize,args)
    def SetTextXY(*args): return apply(mindiapyc.IDiaOutputWindow_SetTextXY,args)
    def GetTextCount(*args): return apply(mindiapyc.IDiaOutputWindow_GetTextCount,args)
    def MoveText(*args): return apply(mindiapyc.IDiaOutputWindow_MoveText,args)
    def GetTextX(*args): return apply(mindiapyc.IDiaOutputWindow_GetTextX,args)
    def GetTextY(*args): return apply(mindiapyc.IDiaOutputWindow_GetTextY,args)
    def GetTextWidth(*args): return apply(mindiapyc.IDiaOutputWindow_GetTextWidth,args)
    def GetTextHeight(*args): return apply(mindiapyc.IDiaOutputWindow_GetTextHeight,args)
    def SetTextColor(*args): return apply(mindiapyc.IDiaOutputWindow_SetTextColor,args)
    def GetTextColor(*args): return apply(mindiapyc.IDiaOutputWindow_GetTextColor,args)
    def DeleteText(*args): return apply(mindiapyc.IDiaOutputWindow_DeleteText,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C IDiaOutputWindow instance at %s>" % (self.this,)

class IDiaOutputWindowPtr(IDiaOutputWindow):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = IDiaOutputWindow
mindiapyc.IDiaOutputWindow_swigregister(IDiaOutputWindowPtr)
class IDia:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,IDia):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = IDia.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = IDia.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __del__(self,mindiapyc=mindiapyc):
        if getattr(self,'thisown',0):
            mindiapyc.delete_IDia(self)
    def GetTotalTime(*args): return apply(mindiapyc.IDia_GetTotalTime,args)
    def GetShowTime(*args): return apply(mindiapyc.IDia_GetShowTime,args)
    def SetShowTime(*args): return apply(mindiapyc.IDia_SetShowTime,args)
    def GetDissolveTime(*args): return apply(mindiapyc.IDia_GetDissolveTime,args)
    def SetDissolveTime(*args): return apply(mindiapyc.IDia_SetDissolveTime,args)
    def GetId(*args): return apply(mindiapyc.IDia_GetId,args)
    def SetId(*args): return apply(mindiapyc.IDia_SetId,args)
    def GetComment(*args): return apply(mindiapyc.IDia_GetComment,args)
    def SetComment(*args): return apply(mindiapyc.IDia_SetComment,args)
    def GetImageFile(*args): return apply(mindiapyc.IDia_GetImageFile,args)
    def SetImageFile(*args): return apply(mindiapyc.IDia_SetImageFile,args)
    def GetScript(*args): return apply(mindiapyc.IDia_GetScript,args)
    def SetScript(*args): return apply(mindiapyc.IDia_SetScript,args)
    def GetHorizontalFormat(*args): return apply(mindiapyc.IDia_GetHorizontalFormat,args)
    def SetHorizontalFormat(*args): return apply(mindiapyc.IDia_SetHorizontalFormat,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C IDia instance at %s>" % (self.this,)

class IDiaPtr(IDia):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = IDia
mindiapyc.IDia_swigregister(IDiaPtr)
class IDiaPresentation:
    __setmethods__ = {}
    for _s in []: __setmethods__.update(_s.__setmethods__)
    def __setattr__(self,name,value):
        if (name == "this"):
            if isinstance(value,IDiaPresentation):
                self.__dict__[name] = value.this
                if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
                del value.thisown
                return
        method = IDiaPresentation.__setmethods__.get(name,None)
        if method: return method(self,value)
        self.__dict__[name] = value

    __getmethods__ = {}
    for _s in []: __getmethods__.update(_s.__getmethods__)
    def __getattr__(self,name):
        method = IDiaPresentation.__getmethods__.get(name,None)
        if method: return method(self)
        raise AttributeError,name

    def __del__(self,mindiapyc=mindiapyc):
        if getattr(self,'thisown',0):
            mindiapyc.delete_IDiaPresentation(self)
    def IsChanged(*args): return apply(mindiapyc.IDiaPresentation_IsChanged,args)
    def GetDocName(*args): return apply(mindiapyc.IDiaPresentation_GetDocName,args)
    def GetDescription(*args): return apply(mindiapyc.IDiaPresentation_GetDescription,args)
    def GetDiaCount(*args): return apply(mindiapyc.IDiaPresentation_GetDiaCount,args)
    def GetDia(*args): return apply(mindiapyc.IDiaPresentation_GetDia,args)
    def AddNewDia(*args): return apply(mindiapyc.IDiaPresentation_AddNewDia,args)
    def GetSoundFileCount(*args): return apply(mindiapyc.IDiaPresentation_GetSoundFileCount,args)
    def GetSoundFileNameAt(*args): return apply(mindiapyc.IDiaPresentation_GetSoundFileNameAt,args)
    def SetSoundFileNameAt(*args): return apply(mindiapyc.IDiaPresentation_SetSoundFileNameAt,args)
    def GetOutputWindow(*args): return apply(mindiapyc.IDiaPresentation_GetOutputWindow,args)
    def LoadPresentation(*args): return apply(mindiapyc.IDiaPresentation_LoadPresentation,args)
    def SavePresentation(*args): return apply(mindiapyc.IDiaPresentation_SavePresentation,args)
    def SavePresentationAs(*args): return apply(mindiapyc.IDiaPresentation_SavePresentationAs,args)
    def GotoPosition(*args): return apply(mindiapyc.IDiaPresentation_GotoPosition,args)
    def PlayFrom(*args): return apply(mindiapyc.IDiaPresentation_PlayFrom,args)
    def Play(*args): return apply(mindiapyc.IDiaPresentation_Play,args)
    def Stop(*args): return apply(mindiapyc.IDiaPresentation_Stop,args)
    def Pause(*args): return apply(mindiapyc.IDiaPresentation_Pause,args)
    def Continue(*args): return apply(mindiapyc.IDiaPresentation_Continue,args)
    def IsPlayModus(*args): return apply(mindiapyc.IDiaPresentation_IsPlayModus,args)
    def IsPauseModus(*args): return apply(mindiapyc.IDiaPresentation_IsPauseModus,args)
    def IsEditModus(*args): return apply(mindiapyc.IDiaPresentation_IsEditModus,args)
    def GetPlayModusStrg(*args): return apply(mindiapyc.IDiaPresentation_GetPlayModusStrg,args)
    def IsSimulation(*args): return apply(mindiapyc.IDiaPresentation_IsSimulation,args)
    def SetSimulation(*args): return apply(mindiapyc.IDiaPresentation_SetSimulation,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C IDiaPresentation instance at %s>" % (self.this,)

class IDiaPresentationPtr(IDiaPresentation):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = IDiaPresentation
mindiapyc.IDiaPresentation_swigregister(IDiaPresentationPtr)
GetDiaPresentation = mindiapyc.GetDiaPresentation

PrintLn = mindiapyc.PrintLn

DoMessageBox = mindiapyc.DoMessageBox

SleepMS = mindiapyc.SleepMS

RgbToHsv = mindiapyc.RgbToHsv

HsvToRgb = mindiapyc.HsvToRgb


