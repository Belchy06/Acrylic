-- VSCODE MODULE

-- preload
include("_preload.lua")

-- Include workspace and project files
include("vscode_workspace.lua")
include("vscode_project.lua")


-- Aliases
local p = premake
local project = p.project
local vscode = p.extensions.vscode

function vscode.configName(config, includePlatform)
    if includePlatform then
        return config.platform .. "-" .. config.buildcfg
    else
        return config.buildcfg
    end
end

function vscode.getToolsetName(cfg)
    -- MSC for windows, CLANG for everything else
    local default = iif(cfg.system == p.WINDOWS, "msc", "clang")
    return _OPTIONS.cc or cfg.toolset or default
end

function vscode.getCompiler(cfg)
    -- MSC for windows, CLANG for everything else
    local default = iif(cfg.system == p.WINDOWS, "msc", "clang")
    local toolset = p.tools[_OPTIONS.cc or cfg.toolset or default]
    if not toolset then
        error("Invalid toolset '" .. (_OPTIONS.cc or cfg.toolset) "'")
    end
    return toolset
end

function vscode.esc(value)
    value = value:gsub('\\', '\\\\')
    value = value:gsub('"', '\\"')
    return value
end

return vscode
