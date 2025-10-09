- [x] MadSimon replica
- [x] Fix stamina
- [x] Draw info background according to the aspect ratio <- aspect ratio LOLOLOL
- [x] Bhop cap cvar
- [x] Write a minimal SDK

1.0.0
- [x] Use sprites to print out the speedometer
- [x] Hud drawing from the position and color data
- [x] Implement all the cvars
- [x] Parse the res file for the engine font and draw info background accordingly

1.0.1
- [x] Switch to TWHL halflife-updated SDK
- [x] Fix co-op crash due to null dereferences

1.0.2 <-- Skipped
- [ ] Fix fake values (Find a way to get info for the current client no matter the host)

1.1.0
- [x] Switch to Xash3D FWGS SDK
- [x] Autohop
- [x] Ducktap
- [x] +ducktap
- [x] Jumpspeed
- [x] Find out why jumpspeed sometimes doesn't update
- [x] Register commands with flags
- [x] Unregister cvars/commands

1.1.1
- [x] Replace function names with the symbol names in the .so files/SDK
- [x] Completely migrate cxx to c
- [x] Fix autojump in waterlevel 1
- [x] README
- [x] CREDITS
- [x] Tidy up the project structure
	- [x] Separate hooks to multiple files for each hook in hooks.c
	- [x] Organize files to folders
	- [x] Reduce repetitive code like fetching and setting color/position etc.
- [x] Do fixes planned for 1.0.2

# Plans for the next releases
- [ ] Proper logger
- [ ] Make a better signature scanner
- [ ] Beutify the "ugly" parts
	- Ex: 

## Requests
@alex_magnum:
- [ ] Better verification: cvar change detection, command blacklist, script usage detection (should be priority if at least cvar detection is possible),
	- Maybe use whatever check sv_cheat is using and restrict the banned commands? (No idea if it's possible)
		- If possible add indication of cheating
- [ ] System timer is already there, which helps verification but is it possible to somehow add LRT similar to bxt? Would be awesome (but also would probably need to update autosplit  ).,
- [ ] BXT already has some cof cvars, I'm pretty sure it should be easy to implement some of them. It already has
	- [x] infinite stamina
	- [ ] head bob while disable
	- [ ] full stop while pressing use
	- [x] restored ducktap
- [x] I don't know if it's possible but madsimon has problem where you cannot see text, usually happens in the ending, DrLa suggested to put some kind of background behind the madsimon hud if possible.,
- [ ] True HL1 movement. Self-explanatory. 320 movement speed, no dash and sprint, stuff like ducktap and use fullstop. Not a priority but would be nice new category.,

@guru:
- [ ] Save indication text
	- @guru the game already has a "Saved..." subtitle line for when you use a cassette recorder, maybe that could come in handy
- [ ] +left +right
- [ ] noclip

@joah230:
- [ ] Crosshair
	- [x] Color
	- [x] Length
	- [x] Thickness
	- [x] Gap
	- [x] Dot
	- [x] T
	- [x] Dynamic
	- This is already done but for some reason changing the VBO and setting it back to the original one freeze the thread it was called on, and I don't want to use immediate mode gl
		- Try hooking to gl directly (swapbuffers etc.)

# Plans for later
- [ ] Documentation (Doxygen? Wiki?)