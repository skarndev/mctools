Cryect's WoW Tool Release 1.10
August 6th, 2006 (First Release Feb 15th, 2006)

Alright, here's my tools that work sufficiently well to release. They are all compiled with cygwin but you should be able to recompile them with your favorite C++ compiler.

mapmover <MapName> <LowerX> <LowerY> <UpperX> <UpperY> <NewMapName> <OffsetX> <OffsetY> - Tool that just makes it really easy to move files and renames them for you
createwdt <MapName> <LowerX> <LowerY> <UpperX> <UpperY> - Creates a valid WDT file for the specified map name for a rectangle of ADT's between (LowerX,LowerY) to (UpperX, UpperY)
offsetfix <MapName> <ZOffset Optional Default=0> - Corrects ADT tiles after moving them around, also can adjust the Z Height so you don't fall to your death etc when you zone into an instance
FileInfo <adt File> - Outputs all the ADT info about textures used, MD2's used, where the MD2's are, WMO's used, WMO Info
ModelSwap <ADT File> <MDX Model to Replace> <MDX Model to Swap In> - Instead of Swapping the models and changing every single place where its located you can instead modify the appropriate ADT file to swap the model there. The only requirement is the string length of the replacement model be shorter or equal length of the model you are replacing.
RemoveTheWalls <ADT File> - Disables all invisible wall chunks in the ADT file.
LoadInfo <adt file> - Takes the info outputted from FileInfo and reloads it into a file be careful minor errors result in major issues
PatchHoles <adt file> - Removes all the holes from a tile
AllWater <adt file> <water level> <optional waterflags default 14> - Allows you to set the entire water of an ADT tile to the specified level
WMOOutdoors - Converts a WMO root file (the WMO file without the numbers) so that its outdoors this needed to get the water effect inside a WMO though at this time you don't get the water physics
MakeMap <InstanceName> <LowerX> <LowerY> <Width> <Height> - Composites all the RAW files outputted by wowmapview into one large raw map file
M2Modder <M2 FileName> - This lets you edit a lot of properties with M2 model files though its limited in a lot of ways its still a very powerful tool for model existing M2's
PExtracter <M2 Filename> - Extracts all the particles from a file to a seperate file just containing the particles
PInjector <M2 Filename> <Particle Filename> - Replaces the particles of the model with the particles from the particle file.
LayerAdder <M2 Filename> - Adds a second layer to a model based on a specified texture. (doesn't add anything else which you might need to do by hand)
LightsOut <WMO Filename> - Sets all lights from WMO's of both the root & group to pitch black (will later set it up so you can specify the color)