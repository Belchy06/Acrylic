local p = premake
local project = p.project

-- Initialize extension
p.extensions.vscode = premake.extensions.vscode or {}
local vscode        = p.extensions.vscode
vscode._VERSION     = premake._VERSION

require('vstudio')

newoption {
    trigger = "enable_prereleases",
    description = "Enable prerelease versions of Visual Studio"
}

newaction {
    -- Metadata
    trigger = "vscode",
    shortname = "VS Code",
    description = "Generate Visual Studio Code Projects",

    -- Capabilities
    valid_kinds = { "ConsoleApp", "WindowedApp", "Makefile", "SharedLib", "StaticLib", "Utility" },
    valid_languages = { "C", "C++", "C#" },
    valid_tools = { "gcc", "clang", "msc" },

    -- Generate appropriate build files
    onStart = function()
        if _TARGET_OS == "windows" then
            os.execute(_PREMAKE_COMMAND .. " vs2022")
        else
            os.execute(_PREMAKE_COMMAND .. " gmake2")
        end
    end,

    -- Workspace Generation
    onWorkspace = function(wks)
        p.eol("\r\n")    
    	p.indent("\t")
    	-- NOTE: the .code-workspace file contains the tasks and launch configurations.
    	p.generate(wks, ".code-workspace", vscode.workspace.generate)
    end,

    -- Project Generation
    onProject = function(prj)
        p.eol("\r\n")    
    	p.indent("\t")

    	-- C/C++ Support
    	-- *    This is where support for other languages will eventually be specified
    	if (project.isc(prj) or project.iscpp(prj)) then
        	p.generate(prj, prj.location .. "/.vscode/c_cpp_properties.json", vscode.project.cCppProperties.generate)
    	end
    end
}

return function(cfg)
    return _ACTION == "vscode"
end