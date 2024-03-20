local graphics <const> = playdate.graphics
local geometry <const> = playdate.geometry
local sprite   <const> = playdate.sprite
local sys      <const> = playdate.system

function playdate.update()
    sys:c_update()
end
