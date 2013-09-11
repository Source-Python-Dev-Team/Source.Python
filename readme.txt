Source.Python ReadME

What is this?
Source.Python is a Source Engine server plugin that seeks to provide a clean
python interface to the Source Engine and all of its mods.

Directory structure:
addons/ - This is where the python support libraries will go for this plugin.
cfg/ - Any configuration files we want to ship.
src/ - The source code to the plugin.

Source directory structure:
src/core/       - Core plugin code will be placed here. This is where the
                  DLL/SO is built.
src/sdks/       - Place any and all source engine SDKs you want to build
                  against here. Name them in hl2sdk-<game> format.
                  Use http://hg.alliedmods.net/hl2sdks as a reference.
src/thirdparty/ - All third party SDKs such as boost will be placed here.
                  These libraries will be in built form unless
                  we've made changes to their core.