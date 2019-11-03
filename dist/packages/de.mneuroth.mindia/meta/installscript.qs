function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/mindia.exe", "@StartMenuDir@/MinDia.lnk",
            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll",
            "iconId=2", "description=MinDia");
        component.addOperation("CreateShortcut", "@TargetDir@/mindia.exe", "@DesktopDir@/MinDia.lnk",
            "workingDirectory=@TargetDir@");
    }
}