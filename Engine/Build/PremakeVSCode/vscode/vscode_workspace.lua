-- INCLUDES
include("vscode_tasks.lua")
include("vscode_launch.lua")

local p = premake
local tree = p.tree
local vscode = p.extensions.vscode


-- WORKSPACE FILE
vscode.workspace = {}

function vscode.workspace.generateFolders(wks)
    p.push('"folders": [')

    -- Workspace vscode folder
    p.push('{')
    p.w('"path": "."')
    p.pop('},')

    p.pop('],')
end

-- WORKSPACE AND TASK GENERATION
function vscode.workspace.generate(wks)
    p.push('{')

    vscode.workspace.generateFolders(wks)
    vscode.tasks.generate(wks)
    -- Check if there is a startup project specified
    if not (wks.startproject == nil or wks.startproject == "") then
        vscode.launch.generate(wks.projects[wks.startproject]) -- launch/startup project
    -- HACK If no startproject was specified, just generate for the first project in the workspace
    else
        prj = wks.projects[1]
        if not (prj == nil) then
            vscode.launch.generate(prj)
        end
    end

    p.pop('}')
end
