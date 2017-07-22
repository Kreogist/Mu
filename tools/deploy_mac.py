# -*- coding: utf-8 -*-
import sys
import json
import os
import distutils.dir_util
import xml.dom.minidom

deploy_cache = {}
use_deploy_cache = True
cache_file_path=os.path.abspath(__file__+".user")

def parse_pro_user(user_file_path):
    # Check the user file exist.
    if not os.path.exists(user_file_path):
        print("Cannot find mu.pro.user, please compile µ with Qt Creator.\n")
        return []
    # Read the pro.user file.
    user_file_tree = xml.dom.minidom.parse(user_file_path)
    user_file_root = user_file_tree.documentElement
    # Prepare the target list.
    target_list = []
    # Inside the document element, we have to find the node ProjectExplorer.Project.TargetCount.
    for user_node in user_file_root.childNodes:
        if user_node.localName=="data" and isinstance(user_node, (xml.dom.minidom.Element)):
            # Get the node.
            data_node=user_node
            # The name of the node is ProjectExplorer.Project.Target.x, check the name.
            name_element=data_node.getElementsByTagName("variable")
            if len(name_element)!=1:
                continue
            name_element=name_element[0]
            if name_element.childNodes[0].data.startswith("ProjectExplorer.Project.Target."):
                # Find the target.
                target_list.append(data_node)
    # Check the target list result.
    if len(target_list)==0:
        print("Failed to find any target from the result.")
        return []
    # Prepare the target info list.
    target_info_list = []
    # Loop to find all the target data.
    for target_element in target_list:
        target_info = {}
        # Get the value map.
        for target_element_child in target_element.childNodes:
            if target_element_child.localName=="valuemap" and isinstance(target_element_child, (xml.dom.minidom.Element)):
                # Find the value map.
                target_map = target_element_child
                # Loop, and find all the valuemap in the target map. Pick out the build configuration.
                build_configure_list = []
                for target_map_data in target_map.childNodes:
                    if not isinstance(target_map_data, (xml.dom.minidom.Element)):
                        continue
                    # Check map data result.
                    if target_map_data.localName=="valuemap":
                        # Check the name of the map data.
                        if target_map_data.hasAttribute('key') and target_map_data.getAttribute('key').startswith('ProjectExplorer.Target.BuildConfiguration.'):
                            # Save the result in build configure list.
                            build_configure_list.append(target_map_data)
                    elif target_map_data.localName=="value" and target_map_data.getAttribute('key')=='ProjectExplorer.ProjectConfiguration.DefaultDisplayName':
                        target_info['Name']=target_map_data.childNodes[0].data
                # Check the build configure list data.
                if len(build_configure_list)==0:
                    # No need to check the other data.
                    break
                # Check the build directory, name of the build.
                build_info_list = []
                for build_valuemap in build_configure_list:
                    build_info = {}
                    # Loop in the build value mape.
                    for build_value in build_valuemap.childNodes:
                        # Get the build value data.
                        if build_value.localName=="value" and isinstance(build_value, (xml.dom.minidom.Element)):
                            # Check the build value type and key.
                            if build_value.getAttribute("type")=="QString":
                                # Check the key name.
                                if build_value.getAttribute("key")=="ProjectExplorer.ProjectConfiguration.DefaultDisplayName":
                                    build_info['Name']=build_value.childNodes[0].data
                                elif build_value.getAttribute("key")=="ProjectExplorer.BuildConfiguration.BuildDirectory":
                                    build_info['Dir']=build_value.childNodes[0].data
                    # Check the build info.
                    if not 'Name' in build_info or not 'Dir' in build_info:
                        continue
                    # Add the build info to build info list.
                    build_info_list.append(build_info)
                # Append the data to target info list.
                if len(build_info_list)==0:
                    break
                # Add build info list to target info.
                target_info['Builds']=build_info_list
                # Add the target info to list.
                target_info_list.append(target_info)
                # Mission complete for the current element.
                break
    return target_info_list
    
def get_build_target_dir(target_info):
    global deploy_cache
    # Check the target info result.
    if len(target_info)==0:
        print("Failed to find any project target info.")
        return ""
    # Print the target info result.
    selected_target=-1
    print("Find "+str(len(target_info))+" project target(s):\n  #  Target Name")
    cached_target_name=""
    if 'Target' in deploy_cache:
        cached_target_name=deploy_cache['Target']
    for i in range(0, len(target_info)):
        index_str="%3d  " %(i+1)
        print(index_str+target_info[i]['Name'])
        if target_info[i]['Name']==cached_target_name:
            selected_target=i
    # Check the target info length.
    if len(target_info)==1:
        selected_target=0
    else:
        if selected_target==-1:
            print("Please input the index of the target for the deployment: ", end='')
            selected_target=input()
            selected_target=int(selected_target)-1
            if selected_target < 0 or selected_target > len(target_info):
                print("Invalid target index.")
                return ""
    deploy_cache['Target']=target_info[selected_target]['Name']
    # Pick out the selected target.
    deploy_target=target_info[selected_target]
    print("Use target '"+deploy_target['Name']+"' for deployment.")
    # Check the builds.
    print("Target '"+deploy_target['Name']+"' supports for the following build(s):\n  #  Build Name")
    build_info=deploy_target['Builds']
    selected_build=-1
    cached_build_name=""
    if 'Build' in deploy_cache:
        cached_build_name=deploy_cache['Build']
    for i in range(0, len(build_info)):
        index_str="%3d  " %(i+1)
        print(index_str+build_info[i]['Name'])
        if cached_build_name==build_info[i]['Name']:
            selected_build=i
    # Check target build count.
    if len(build_info)==1:
        selected_build=0
    else:
        if selected_build==-1:
            print("Please input the index of the build for the deployment: ", end='')
            selected_build=input()
            selected_build=int(selected_build)-1
            if selected_build < 0 or selected_build >= len(build_info):
                print("Invalid build index.")
                return ""
    deploy_cache['Build']=build_info[selected_build]['Name']
    print("Using '"+build_info[selected_build]['Name']+"' as the build for the deployment.")
    return build_info[selected_build]['Dir']

def main():
    global use_deploy_cache
    global deploy_cache
    # Find the .pro.user file.
    source_dir=os.path.abspath(os.path.join(sys.path[0],".."))
    # Parse the target information.
    target_info=parse_pro_user(os.path.join(source_dir, "mu.pro.user"))
    if len(target_info)==0:
        return False
    # Get the build target dir.
    build_target_dir=get_build_target_dir(target_info)
    if build_target_dir=="":
        return False
    # Ask for the Qt 
    cached_bin_path=""
    if 'QtBin' in deploy_cache:
        cached_bin_path=deploy_cache['QtBin']
    cached_path_check=os.path.exists(cached_bin_path)
    qt_bin_path=cached_bin_path
    if len(qt_bin_path)==0 or not use_deploy_cache or not cached_path_check:
        if cached_path_check:
            print("Cached Qt bin path is not existed.")
        print("Please input the Qt bin directory path", end='')
        if len(cached_bin_path)>0:
            print("["+cached_bin_path+"]", end='')
        print(": ", end='')
        qt_bin_path=input()
    else:
        print("Qt bin path is: " + qt_bin_path)
    # Check the bin path.
    if len(qt_bin_path)==0:
        qt_bin_path=cached_bin_path
    # Check the path existence.
    if not os.path.exists(qt_bin_path):
        print("The Qt bin path is not exist.")
        return False
    deploy_cache['QtBin']=qt_bin_path
    # Get the output path.
    cached_output_path=""
    if 'Output' in deploy_cache:
        cached_output_path=deploy_cache['Output']
    output_path=cached_output_path
    if len(output_path)==0 or not use_deploy_cache:
        print("Please input the output directory path: ", end='')
        output_path=input()
    else:
        print("Output path is: "+output_path)
    # Check the output path existence.
    if not os.path.exists(output_path):
        # Build the output path.
        distutils.dir_util.mkpath(output_path)
    deploy_cache['Output']=output_path
    # Copy the mu.app to the output path.
    mu_app_parent_path=os.path.join(build_target_dir, "src")
    mu_app_path=os.path.join(mu_app_parent_path, "mu.app")
    # Check the mu app path.
    print("Checking the mu.app folder...", end='')
    if not os.path.exists(mu_app_path):
        print("\nError: Failed to find mu.app directory.")
        return False
    # Executing the copy path.
    print("Done")
    # Check the existence of the mu.app at the target position.
    target_folder_position=os.path.join(output_path, "mu.app")
    print("The output path is: "+target_folder_position)
    if os.path.exists(target_folder_position):
        # Remove the mu.app folder.
        print("Removing the target mu.app folder...", end='')
        os.system('rm -rf '+target_folder_position)
        if os.path.exists(os.path.join(output_path, "mu.app")):
            print("\nError: Failed to remove the target mu.app folder.")
            return False
        print("Done")
    # Copy the application folder to target path.
    print("Copy the mu.app to the target path...", end='')
    os.system('cp -rf \"'+mu_app_path + '\" \"' + output_path+'\"')
    print("Done")
    # Use macdeployqt from the Qt bin directory to deploy the mu.app.
    print("Executing macdeployqt...", end='')
    macdeployqt_path=os.path.join(qt_bin_path, 'macdeployqt')
    os.system(macdeployqt_path+' '+target_folder_position)
    print("Done")
    # Copy the libbass.dylib from the system path.
    print("Copy the libbass.dylib to the target path...", end='')
    if not os.path.exists('/usr/local/lib/libbass.dylib'):
        print("\nError: Cannot find libbass.dylib from /usr/local/lib.")
        return False
    os.system('cp /usr/local/lib/libbass.dylib '+target_folder_position+'/Contents/MacOS')
    print("Done")
    # Use install name tool to change the libbass.dylib path.
    print("Changing the libbass.dylib to the target path...", end='')
    os.system('install_name_tool -change @loader_path/libbass.dylib @executable_path/../MacOS/libbass.dylib '+target_folder_position+'/Contents/MacOS/mu')
    print("Done")
    # Released language list.
    language_list=['Simplified_Chinese', 'Traditional_Chinese', 'Japanese']
    lrelease_path = os.path.join(qt_bin_path, 'lrelease')
    language_target_path=target_folder_position+'/Contents/Resources/Language'
    # Build the language path.
    if not os.path.exists(language_target_path):
        distutils.dir_util.mkpath(language_target_path)
    # Release the langauge translation.
    print("Releasing language file...")
    for language in language_list:
        release_translation(lrelease_path, source_dir, language, language_target_path)
    print("Language update complete.")
    return True
    
def release_translation(lrelease_path, source_dir, language_name, target_path):
    print("Processing " + language_name + " ...", end='')
    # Build the target language path.
    language_path=os.path.join(target_path, language_name)
    # Check path existence.
    if not os.path.exists(language_path):
        distutils.dir_util.mkpath(language_path)
    # Get the language source path.
    language_source = source_dir+"/src/i18n/"
    # Copy the image to the path.
    os.system('cp '+language_source+language_name+'.png '+language_path+'/'+language_name+'.png')
    # Release the qm file to the language path.
    os.system(lrelease_path+' -silent '+language_source+language_name+'.ts -qm '+language_path+'/'+language_name+'.qm')
    print("Done")

def parse_parameter():
    global use_deploy_cache
    # Get the parameters.
    print(sys.argv)
    configureList=list(sys.argv[1:])
    while len(configureList) > 0:
        # Get the first item in the list.
        parameter = configureList[0]
        configureList = configureList[1:]
        # Check the parameter.
        if parameter == "-h" or parameter == "--help":
            # Display the error information.
            print("Options:")
            print("-h, --help       Display this information")
            print("-n, --no-cache   Do not use the cache file configure")
            print("-o, --output     The output path of the application")
            print("-q, --qt-bin     The path of the Qt bin")
            print("-v, --version    Display the version of this tool")
            return False
        elif parameter == "-n" or parameter == "--no-cache":
            use_deploy_cache = False
        elif parameter == "-q" or parameter == "--qt-bin":
            use_deploy_cache['QtBin']=configureList[0]
            configureList = configureList[1:]
        elif parameter == "-o" or parameter == "--output":
            use_deploy_cache['Output']=configureList[0]
            configureList = configureList[1:]
        elif parameter == "-v" or parameter == "--version":
            return False
        else:
            print("Unknown parameter: " + parameter)
            return False
    return True

def save_cache_data():
    global deploy_cache
    global cache_file_path
    # Read the file to cache string.
    cache_file=open(cache_file_path, 'w')
    cache_file.write(json.dumps(deploy_cache))
    cache_file.close()

def load_cache_data():
    global deploy_cache
    global cache_file_path
    # Check the cache file existence.
    if not os.path.exists(cache_file_path):
        return
    # Read the file to cache string.
    cache_file=open(cache_file_path, 'r')
    cache_file_text=cache_file.read()
    cache_file.close()
    # Parse the cache file text to deploy cache.
    deploy_cache=json.loads(cache_file_text)

if __name__=="__main__":
    # Try to load cache data.
    load_cache_data()
    # Parse parameters.
    if parse_parameter():
        # Start main process.
        main()
        print("Complete.")
    # Save the cache data.
    save_cache_data()
