import subprocess
import os
import sys
import httplib
import zipfile
import shutil
import tempfile

# Command check
def command_exist(command):
    check_process = subprocess.Popen(['command', '-v', command], stdout=subprocess.PIPE)
    stdout=check_process.communicate()[0]
    return len(stdout)!=0

# Command dependency check and install
def check_dependency(name, command, install):
    print "Checking", name, "...",
    # If command exists, nothing happend
    if command_exist(command):
        print "Done"
        return True
    print "Failed"
    # Command not exist, install the command.
    print "Installing " + name + "..."
    sys.stdout.flush()
    os.system(install)
    # Check the comand after install.
    if not command_exist(command):
        print "Failed to install ", name, "."
        return False
    # Success.
    print name, " has been installed."
    return True
    
# Check batch dependencies.
def check_dependencies(dependencies):
    for name, command, install in dependencies:
        if not check_dependency(name, command, install):
            return False
    return True

# Get all the installed data in Application.
def find_qt():
    # Prepare the possible list.
    installation_list=[]
    # Find all folder contains Qt.
    for app_name in os.listdir('/Applications'):
        app_path='/Applications/'+app_name
        if "Qt" in app_name and os.path.exists(app_path):
            # Possible Qt installtion, get the version.
            qt_version=app_name[2:5]
            # The path should be app_path with version in it.
            qt_bin_path=app_path+'/'+qt_version+'/clang_64/bin'
            # Check directory
            if os.path.exists(qt_bin_path) and os.path.isfile(qt_bin_path+'qmake'):
                # Save installation data.
                installation_list.append((qt_version, qt_bin_path))
    # Give back the list.
    return installation_list

# Check bass.
def check_bass_lib():
    print 'Checking bass library...',
    # Check the bass library.
    if os.path.isfile('/usr/local/include/bass.h') and os.path.isfile('/usr/local/lib/libbass.dylib'):
        print 'Done'
        return True
    # Download bass.zip from un4seen
    conn = http.client.HTTPConnection('www.un4seen.com')
    conn.request("GET", "/files/bass24-osx.zip")
    response = conn.getresponse()
    if response.status != 200:
        print "Failed to download bass library file."
        return False
    bass_zip_data = response.read()
    conn.close()
    print "Done, ", len(bass_zip_data), " bytes downloaded."
    bass_zip_file=tempfile.TemporaryFile()
    bass_zip_file.write(bass_zip_data)
    print "Extracting file..."
    with zipfile.ZipFile(bass_zip_file, 'r') as bass_zip:
        bass_zip_infolist = bass_zip.infolist()
        for bass_zip_item in bass_zip_infolist:
            print bass_zip_item.filename

# Main function.
def main():
    # Check dependencies
    dependencies = [
        ("Homebrew", "brew", '/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"'),
        ("FFMpeg", "ffmpeg", 'brew install ffmpeg')]
    if not check_dependencies(dependencies):
        return False
    # Check bass installation.
    if not check_bass_lib():
        return False
    # Check Qt installation.
    qt_installation=find_qt()
    # Check the installation list.
    if len(qt_installation)==0:
        print "Cannot find any Qt installation."
        # Add customized Qt installation path.

if __name__ == "__main__":
    # execute only if run as a script
    main()
