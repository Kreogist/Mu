#!/usr/bin/python3
#-*-coding: utf-8 -*-
import os
import sys
import http.client
from html.parser import HTMLParser

gpg_key = ""

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
for element in element_list:
	# Get the attrs.
	element_attrs = list(element["attrs"][0])
	# Check the elements.
	if element_attrs[0] != "href":
		continue
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
for ubuntu_version in ubuntu_release:
	print("Uploading " + ubuntu_version + "...")
	print("Working on changelong...")	
	change_log_file = open('changelog', 'w')
	change_log_file.write(change_log_first_line[:end_bracket] + '-' + \
	 					  ubuntu_version + ") " + ubuntu_version + \
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
	print("$ dput -f ppa:kreogistdevteam/mu ../kreogist-mu_"+current_mu_version+"-"+ ubuntu_version + "_source.changes")
	os.system("dput -f ppa:kreogistdevteam/mu ../kreogist-mu_"+current_mu_version+"-"+ ubuntu_version + "_source.changes")
	# Clean up.
	print("Cleaning up...")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ ubuntu_version +".dsc")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ ubuntu_version +"_source.build")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ ubuntu_version +"_source.changes")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ ubuntu_version +".tar.xz")
	os.system("rm -f ../kreogist-mu_"+current_mu_version+"-"+ ubuntu_version +"_source.ppa.upload")
	# Done.
	print("Upload " + ubuntu_version + " done.")
	# Back to script root.
	os.chdir(os.path.dirname(script_path))
	
# Write back the changelog
change_log_file = open('changelog', 'w')
change_log_file.write(change_log_first_line + '\n' + change_log_body)
change_log_file.close()
