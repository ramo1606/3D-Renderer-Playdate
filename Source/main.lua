import "CoreLibs/graphics"

local pd <const> = playdate
local gfx <const> = pd.graphics

local playerX, playerY = 200, 120
local playerRadius = 15
local playerSpeed = 1.5

function pd.update()
    gfx.clear()
    local crankAngle = math.rad(pd.getCrankPosition())
    playerX += math.sin(crankAngle) * playerSpeed
    playerY -= math.cos(crankAngle) * playerSpeed
    gfx.fillCircleAtPoint(playerX, playerY, playerRadius)
end