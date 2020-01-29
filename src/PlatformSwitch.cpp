/*
Copyright © 2016 Justin Jacobs

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

#ifdef PLATFORM_CPP_INCLUDE
#ifndef PLATFORM_CPP
#define PLATFORM_CPP

#include "Platform.h"
#include "Settings.h"
#include "SharedResources.h"
#include "Utils.h"
#include "UtilsFileSystem.h"
#include "UtilsParsing.h"

#include <switch.h>

#include <SDL2/SDL.h>

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

Platform platform;

Platform::Platform()
	: has_exit_button(true)
	, is_mobile_device(false)
	, force_hardware_cursor(true)
	, has_lock_file(false)
	, config_menu_type(CONFIG_MENU_TYPE_DESKTOP) ///CONFIG_MENU_TYPE_DESKTOP_NO_VIDEO,CONFIG_MENU_TYPE_DESKTOP,CONFIG_MENU_TYPE_BASE;
	, default_renderer("sdl_hardware")
	, config_video(Platform::Video::COUNT, true)
	, config_audio(Platform::Audio::COUNT, true)
	, config_interface(Platform::Interface::COUNT, true)
	, config_input(Platform::Input::COUNT, true)
	, config_misc(Platform::Misc::COUNT, true)
{
	config_video[Platform::Video::RENDERER] = false;
	config_video[Platform::Video::FULLSCREEN] = false;
	config_video[Platform::Video::HWSURFACE] = false;
	config_video[Platform::Video::VSYNC] = false;
	config_video[Platform::Video::TEXTURE_FILTER] = false;
	config_video[Platform::Video::ENABLE_GAMMA] = false;
	config_video[Platform::Video::GAMMA] = false;

	config_interface[Platform::Interface::HARDWARE_CURSOR] = false;

	config_input[Platform::Input::JOYSTICK] = false;
	config_input[Platform::Input::MOUSE_MOVE] = false;
	config_input[Platform::Input::MOUSE_AIM] = false;
	config_input[Platform::Input::NO_MOUSE] = false;
	config_input[Platform::Input::MOUSE_MOVE_SWAP] = false;
	config_input[Platform::Input::MOUSE_MOVE_ATTACK] = false;
	config_input[Platform::Input::JOYSTICK_DEADZONE] = true;

	config_misc[Platform::Misc::KEYBINDS] = true;
}

Platform::~Platform() {
}

void Platform::setPaths() {

	settings->path_user = "/switch/flare/";
	Filesystem::createDir(settings->path_user);
	settings->path_conf = settings->path_user + "config/";
	Filesystem::createDir(settings->path_conf);
	
	Filesystem::createDir(settings->path_user + "mods/");
	Filesystem::createDir(settings->path_user + "saves/");

	// data folder

	// these flags are set to true when a valid directory is found
	bool path_data = false;

	// Check for the local data before trying installed ones.
	if (Filesystem::pathExists("./mods")) {
		if (!path_data) settings->path_data = "./";
		path_data = true;
	}

	// finally assume the local folder
	if (!path_data)	settings->path_data = "./";
}

bool Platform::dirCreate(const std::string& path) {
	if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
		std::string error_msg = "createDir (" + path + ")";
		perror(error_msg.c_str());
		return false;
	}
	return true;
}

bool Platform::dirRemove(const std::string& path) {
	if (rmdir(path.c_str()) == -1) {
		std::string error_msg = "removeDir (" + path + ")";
		perror(error_msg.c_str());
		return false;
	}
	return true;
}

// unused
void Platform::FSInit() {}
bool Platform::FSCheckReady() { return true; }
void Platform::FSCommit() {}
void Platform::setExitEventFilter() {}

void Platform::setScreenSize() {
	// can't change window size dynamically with Switch, so default to 16:9 aspect ratio
	settings->screen_w = 1920;
	settings->screen_h = 1080;
}
#endif // PLATFORM_CPP
#endif // PLATFORM_CPP_INCLUDE
