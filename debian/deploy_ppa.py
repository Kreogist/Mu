#!/usr/bin/python3
#-*-coding: utf-8 -*-
import os
import sys
import http.client
from html.parser import HTMLParser

gpg_key = ""
src_pro_path = "../src/src.pro"
prj_lines = []
target_line = -1
prefix = ""

configure_map = {
	"": ["backend-mpv", "ffmpeg-swresample", "transcoder-ffmpeg"],
	"14": ["backend-mpv", "ffmpeg-avresample"],
	"12": ["backend-gstreamer", "ffmpeg-avresample"]
}

class UbuntuListPatcher(HTMLParser):
	def __init__(self):
		super(UbuntuListPatcher, self).__init__()
		self.tags = []
		self.tag_data = {}

	def handle_starttag(self, tag, attrs):
		self.tag_data = {"name": tag, "attrs": attrs}

	def handle_endtag(self, tag):
		self.tags.append(self.tag_data)

	def handle_data(self, data):
		self.tag_data["data"] = data;

	def parsed_data(self):
		return self.tags;


def getUbuntuVersion(versionString):
	if "." in versionString:
		dotPos = versionString.index(".")
		return versionString[:dotPos], versionString[dotPos+1:]
	return versionString, ""

def getConfigure(versionString):
	major_v, minor_v = getUbuntuVersion(versionString)
	if major_v in configure_map:
		return " ".join(configure_map[major_v])
	return " ".join(configure_map[""])

def cachePrjFile():
	global prj_lines
	global target_line
	global prefix
	global src_pro_path
	prj_file = open(src_pro_path)
	try:
		prj_content = prj_file.read()
	finally:
		prj_file.close()
	prj_lines = prj_content.split('\n')
	target_line = -1
	config_comment = "# Backend and ffmpeg configuration for linux."
	for i in range(0, len(prj_lines)):
		strip_line = prj_lines[i].strip()
		if strip_line == config_comment:
			target_line = i+1
			break
	if "+=" in prj_lines[target_line]:
		prefix = prj_lines[target_line][:prj_lines[target_line].index("+=")] + "+= "

# Cache the project file.
cachePrjFile()

# Get the PGP key.
if len(sys.argv) != 2:
	print("Usage: python3 " + sys.argv[0] + " <GPG key>")
	exit()
gpg_key = sys.argv[1]
# Get official releases.
ubuntu_release = http.client.HTTPConnection("releases.ubuntu.com")
ubuntu_release.request("GET", "/")
ubuntu_release = ubuntu_release.getresponse()
ubuntu_release = str(ubuntu_release.read())
# Find out all the code name of the release OS.
pre_element_pos = ubuntu_release.index('<pre>')
if pre_element_pos < 0:
	print("Failed to find release code list.")
	exit()
# Get the element list.
pre_element_end = ubuntu_release.index('</pre>', pre_element_pos)
pre_element = ubuntu_release[pre_element_pos+5:pre_element_end]
parser = UbuntuListPatcher()
parser.feed(pre_element)
element_list = parser.parsed_data()[4:]
element_list.reverse()
ubuntu_release = []
ubuntu_version = []
for element in element_list:
	# Get the attrs.
	element_attrs = list(element["attrs"][0])
	# Check the elements.
	if element_attrs[0] != "href":
		continue
    # Find the Ubuntu version.
	ubuntu_pos=element["data"].find("Ubuntu")
	if ubuntu_pos == -1:
		continue
	element_version=element["data"][ubuntu_pos+7:ubuntu_pos+12]
	# The second data should be the version data.
	element_href = element_attrs[1][:-1]
	# Check the data.
	if element_href == "ubuntu-core":
		continue
	# When it first reaches a number, finished.
	first_asc = ord(element_href[0])
	if first_asc > 47 and first_asc < 58:
		break
	# Get all the codes here.
	ubuntu_release.append(element_href)
	ubuntu_version.append(element_version)
# Read the changelog
change_log_file = open('changelog', 'r')
change_log_data = change_log_file.read();
change_log_file.close()
# Find the first line.
change_log_first_line_end=change_log_data.find('\n')
change_log_first_line=change_log_data[:change_log_first_line_end]
change_log_body=change_log_data[change_log_first_line_end+1:]
start_bracket=change_log_first_line.find("(")
end_bracket=change_log_first_line.find(")")
end_series=change_log_first_line.find(";")
current_mu_version = change_log_first_line[start_bracket+1:end_bracket]
# Print data.
print("Upload information:")
print("Version   : " + current_mu_version)
print("Releases  : " + ', '.join(ubuntu_release))
# We would insert some data into first line.
for i in range(0, len(ubuntu_release)):
	upload_version = ubuntu_release[i]
	print("Uploading " + upload_version + "...")
	print("Configuring the project file...")
	major_v, minor_v = getUbuntuVersion(ubuntu_version[i])
	# Generate the project file for the version.
	prj_lines[target_line] = prefix + getConfigure(major_v)
	prj_content = "\n".join(prj_lines)
	prj_file = open(src_pro_path, "w", encoding = "utf-8")
	try:
		prj_file.write(prj_content)
	finally:
		prj_file.close()
	print("Working on changelong...")	
	change_log_file = open('changelog', 'w')
	change_log_file.write(change_log_first_line[:end_bracket] + '-' + \
	 					  upload_version + ") " + upload_version + \
						  change_log_first_line[end_series:] + \
						  '\n' + change_log_body)
	change_log_file.close()
	# Change to source code root.
	script_path = os.path.abspath(__file__)
	repository_path = os.path.abspath(os.path.join(os.path.dirname(script_path), os.pardir))
	os.chdir(repository_path)
	# Execute the debuild.
	sys.stdout.flush()
	print("Launching debuild...")
	print("$ debuild -S -sa -k" + gpg_key)
	os.system("debuild -S -sa -k" + gpg_key)
	# Execute the dput to upload the data.
	print("Launching dput...")
	print("$ dput -f ppa:kreogistdevteam/mu ../kreogist-mu_"+current_mu_version+"-"+ upload_version + "_source.changes")
	os.system("dput -f ppa:kreogistdevteam/mu ../kreogist-mu_"+current_mu_version+"-"+ upload_version + "_source.changes")
	# Clean up.
	print("Cleaning up...")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ upload_version +".dsc")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ upload_version +"_source.build")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ upload_version +"_source.changes")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ upload_version +".tar.xz")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ upload_version +"_source.ppa.upload")
	# Done.
	print("Upload " + upload_version + " done.")
	# Back to script root.
	os.chdir(os.path.dirname(script_path))
	
# Write back the project file
prj_lines[target_line] = prefix + getConfigure("")
prj_content = "\n".join(prj_lines)
prj_file = open(src_pro_path, "w", encoding = "utf-8")
try:
	prj_file.write(prj_content)
finally:
	prj_file.close()
# Write back the changelog
change_log_file = open('changelog', 'w')
change_log_file.write(change_log_first_line + '\n' + change_log_body)
change_log_file.close()
