local graphics <const> = playdate.graphics
local geometry <const> = playdate.geometry
local sprite   <const> = playdate.sprite
local sys      <const> = playdate.system
local display  <const> = playdate.display

local initialized = false;
local native = c_game.game_new();

function playdate.update()
    if not initialized then
        native:game_initialize()
        initialized = true;
    else
        local dt <const> = 1.0 / display.getRefreshRate()
        native:game_update(dt)
        native:game_draw()
    end
end