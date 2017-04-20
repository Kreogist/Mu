import tempfile
import http.client
import shutil
import os
import zipfile
import winreg
from os import listdir
from os.path import isfile, join

# Parameters

# -a, --all     Compile All
# Compile the project with all the Qt installations.
compile_all = False


# Functions
# copy_content
# Copy one content of folders to another folder.
def copy_content(src, dst):
    names = os.listdir(src)
    if not os.path.exists(dst):
        os.makedirs(dst)
    for name in names:
        srcname = os.path.join(src, name)
        dstname = os.path.join(dst, name)
        if os.path.isdir(srcname):
            shutil.copytree(srcname, dstname)
        else:
            shutil.copy2(srcname, dstname)


print("Searching for Qt installation...", end='')
# Find the Qt installed position, find in registry.
uninst_folder = winreg.OpenKey(winreg.HKEY_CURRENT_USER,
                               r'Software\Microsoft\Windows\CurrentVersion\Uninstall')
key_index = 0
# Prepare the Qt installation path string.
qt_versions = []
while True:
    try:
        folder_key = r'Software\Microsoft\Windows\CurrentVersion\Uninstall' + "\\" + winreg.EnumKey(uninst_folder,
                                                                                                    key_index)
        app_folder = winreg.OpenKey(winreg.HKEY_CURRENT_USER, folder_key)
        sub_key_index = 0
        # Set Qt flags.
        app_data = {}
        qt_inst_loc = ""
        qt_key_find = False
        while True:
            try:
                sub_value = winreg.EnumValue(app_folder, sub_key_index)
                if sub_value[0] == "DisplayName" and ("Qt" in sub_value[1]):
                    qt_key_find = True
                elif sub_value[0] == "InstallLocation":
                    app_data['loc'] = sub_value[1]
                elif sub_value[0] == "DisplayVersion":
                    app_data['version'] = sub_value[1]
                sub_key_index += 1
                continue
            except FileNotFoundError:
                break
            except:
                break
        winreg.CloseKey(app_folder)
        if qt_key_find:
            # Check and find the compiled version.
            # The things we want should be in the folder named with major version
            qt_root_dir = app_data['loc'] + '\\' + app_data['version'][0:3]
            compiler_list = listdir(qt_root_dir)
            if len(compiler_list) != 1:
                # Error.
                continue
            app_data['compiler'] = compiler_list[0]
            qt_root_dir += '\\' + compiler_list[0]
            app_data['root'] = qt_root_dir
            qt_versions.append(app_data)
        key_index += 1
    except:
        break
winreg.CloseKey(uninst_folder)

if len(qt_versions) == "":
    print("\nError: Cannot find Qt installation")
    exit()
print("Done\n")

# Print Qt version installation data.
print("Find the following Qt installation(s):")
app_index = 1
for qt_app in qt_versions:
    print(str(app_index) + ".", qt_app['version'] + ' (' + qt_app['compiler'] + ')', "at", qt_app['loc'])
    app_index += 1

if len(qt_versions) > 1 and not compile_all:
    selected_index = int(input('Select the Qt for the current process: '))
    if selected_index > 0 and selected_index <= len(qt_versions):
        qt_versions = [qt_versions[selected_index - 1]]
    else:
        print("Error: unexpected input.")
        exit()

print("\n Start auto-compiling...")
# Check the file is okay for the current Qt version.
for qt_app in qt_versions:
    print("Compiling with Qt", qt_app['version'])
    print("Checking dependencies...", end='')
    # Checking for ffmpeg files.
    try:
        lib_list = listdir(qt_app['root'] + "\\lib")
        if 'avcodec.lib' not in lib_list:
            print("\nCannot find FFMpeg files, download FFMpeg.")
            # Prepare the URL:
            dev_file_url = ''
            dev_file_name = ''
            dll_file_url = ''
            # Check the Qt bit version:
            print("Checking system version...", end='')
            bit_version = qt_app['compiler'][-2:]
            if bit_version == "64":
                # 64-bit version
                print("OK")
                print("Setting for 64-bit...", end='')
                dev_file_url = "/builds/win64/dev/ffmpeg-latest-win64-dev.zip"
                dev_file_name = "ffmpeg-latest-win64-dev"
                shared_file_url = "/builds/win64/shared/ffmpeg-latest-win64-shared.zip"
                shared_file_name = "ffmpeg-latest-win64-shared"
                print("OK")
            else:
                # 32-bit version
                print("OK")
                print("Setting for 32-bit...", end='')
                dev_file_url = "/builds/win32/dev/ffmpeg-latest-win32-dev.zip"
                dev_file_name = "ffmpeg-latest-win32-dev"
                shared_file_url = "/builds/win32/shared/ffmpeg-latest-win32-shared.zip"
                shared_file_name = "ffmpeg-latest-win32-shared"
                print("OK")
            # Prepare the temp directory.
            work_dir = tempfile.TemporaryDirectory()
            print("Temp working directory: ", work_dir.name)
            # Download the ffmpeg files from the zeranoe.com
            #  Dev pack
            print("Downloading dev files...", end='')
            conn = http.client.HTTPSConnection('ffmpeg.zeranoe.com')
            conn.request("GET", dev_file_url)
            response = conn.getresponse()
            if response.status != 200:
                print("\nFailed to download FFMpeg dev files, check connections.")
                exit()
            ffmpeg_dev_zip = response.read()
            conn.close()
            print("Done, " + str(len(ffmpeg_dev_zip)) + " bytes read.")
            ffmpeg_dev_file = tempfile.TemporaryFile()
            ffmpeg_dev_file.write(ffmpeg_dev_zip)
            print("Extracting files...", end='')
            ffmpeg_dev_include = dev_file_name + '/include/'
            ffmpeg_dev_lib = dev_file_name + '/lib/'
            with zipfile.ZipFile(ffmpeg_dev_file, 'r') as ffmpeg_dev_zip:
                ffmpeg_dev_list = ffmpeg_dev_zip.infolist()
                for ffmpeg_dev_item in ffmpeg_dev_list:
                    if ffmpeg_dev_item.filename.startswith(ffmpeg_dev_include) or ffmpeg_dev_item.filename.startswith(
                            ffmpeg_dev_lib):
                        ffmpeg_dev_zip.extract(ffmpeg_dev_item, work_dir.name)
                print('Done')
            ffmpeg_dev_file.close()
            # Move the file to target position.
            print("Moving files...", end='')
            copy_content(os.path.join(work_dir.name, ffmpeg_dev_include),
                         os.path.join(qt_app['root'], 'include/'))
            copy_content(os.path.join(work_dir.name, ffmpeg_dev_lib),
                         os.path.join(qt_app['root'], 'lib/'))
            print('Done')
            #  Library pack
            print("Downloading shared library files...", end='')
            conn = http.client.HTTPSConnection('ffmpeg.zeranoe.com')
            conn.request("GET", shared_file_url)
            response = conn.getresponse()
            if response.status != 200:
                print("\nFailed to download FFMpeg dev files, check connections.")
                exit()
            ffmpeg_dev_zip = response.read()
            conn.close()
            print("Done, " + str(len(ffmpeg_dev_zip)) + " bytes read.")
            ffmpeg_dev_file = tempfile.TemporaryFile()
            ffmpeg_dev_file.write(ffmpeg_dev_zip)
            print("Extracting files...", end='')
            ffmpeg_shared_bin = shared_file_name + '/bin/'
            ffmpeg_shared_presets = shared_file_name + '/presets/'
            with zipfile.ZipFile(ffmpeg_dev_file, 'r') as ffmpeg_dev_zip:
                ffmpeg_shared_list = ffmpeg_dev_zip.infolist()
                for ffmpeg_shared_item in ffmpeg_shared_list:
                    if ffmpeg_shared_item.filename.startswith(ffmpeg_shared_bin) or ffmpeg_shared_item.filename.startswith(
                            ffmpeg_shared_presets):
                        ffmpeg_dev_zip.extract(ffmpeg_shared_item, work_dir.name)
                print('Done')
            ffmpeg_dev_file.close()
            # Move the file to target position.
            print("Moving files...", end='')
            copy_content(os.path.join(work_dir.name, ffmpeg_shared_bin),
                         os.path.join(qt_app['root'], 'bin/'))
            copy_content(os.path.join(work_dir.name, ffmpeg_shared_presets),
                         os.path.join(qt_app['root'], 'presets/'))
            print('Done')
            # Clear the working directory.
            work_dir.cleanup()

        if 'bass.lib' not in lib_list:
            print("\nCannot find Bass files, download Bass.")
            # Prepare for env vars.
            bass_prefix = ""
            # Check the Qt bit version:
            print("Checking system version...", end='')
            bit_version = qt_app['compiler'][-2:]
            if bit_version == "64":
                # 64-bit version
                print("OK")
                print("Setting for 64-bit...", end='')
                bass_prefix = "x64/"
                print("OK")
            else:
                # 32-bit version
                print("OK")
                print("Setting for 32-bit...", end='')
                bass_prefix = ""
                print("OK")
            print("Downloading Bass file...", end='')
            conn = http.client.HTTPConnection('www.un4seen.com')
            conn.request("GET", "/files/bass24.zip")
            response = conn.getresponse()
            if response.status != 200:
                print("\nFailed to download Bass file, check connections.")
                exit()
            bass_zip = response.read()
            conn.close()
            print("Done, " + str(len(bass_zip)) + " bytes read.")
            bass_zip_file = tempfile.TemporaryFile()
            bass_zip_file.write(bass_zip)
            # Prepare the temp directory.
            work_dir = tempfile.TemporaryDirectory()
            print("Temp working directory: ", work_dir.name)
            # Uncompress files.
            print("Extracting files...", end='')
            # We only need the header, lib files for compile,
            # here also decompress the dll.
            bass_lib_file = "c/" + bass_prefix + "bass.lib"
            bass_inc_file = "c/bass.h"
            bass_dll_file = bass_prefix + "bass.dll"
            with zipfile.ZipFile(bass_zip_file, 'r') as bass_dev_zip:
                bass_zip_list = bass_dev_zip.infolist()
                for bass_zip_item in bass_zip_list:
                    if bass_zip_item.filename == bass_lib_file or bass_zip_item.filename == bass_inc_file or bass_zip_item.filename == bass_dll_file:
                        bass_dev_zip.extract(bass_zip_item, work_dir.name)
                print('Done')
            # Copy the lib and include file to Qt dir.
            print("Moving files...", end='')
            shutil.copy2(os.path.join(work_dir.name, bass_inc_file),
                         os.path.join(qt_app['root'], 'include/'))
            shutil.copy2(os.path.join(work_dir.name, bass_lib_file),
                         os.path.join(qt_app['root'], 'lib/'))
            shutil.copy2(os.path.join(work_dir.name, bass_dll_file),
                         os.path.join(qt_app['root'], 'bin/'))
            print('Done')
            # Clear the working directory.
            work_dir.cleanup()
        print('Done')

    except:
        print("Error happened when checking dependencies")
        exit()
