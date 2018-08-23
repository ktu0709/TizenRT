/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#ifndef __MEDIA_MEDIAPLAYERIMPL_H
#define __MEDIA_MEDIAPLAYERIMPL_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include <media/MediaPlayer.h>
#include <media/InputDataSource.h>
#include <media/MediaPlayerObserverInterface.h>

#include "PlayerObserverWorker.h"

namespace media {
/**
 * @brief current state of MediaPlayer.
 * @details @b #include <media/MediaPlayer.h>
 * @since TizenRT v2.0 PRE
 */
typedef enum player_state_e {
	/** MediaPlayer object was created */
	PLAYER_STATE_NONE,
	/** MediaPlayer worker object was created */
	PLAYER_STATE_IDLE,
	/** MediaPlayer ready to play */
	PLAYER_STATE_READY,
	/** MediaPlayer do playing */
	PLAYER_STATE_PLAYING,
	/** MediaPlayer pause to play */
	PLAYER_STATE_PAUSED
} player_state_t;

typedef enum player_observer_command_e {
	PLAYER_OBSERVER_COMMAND_STARTED,
	PLAYER_OBSERVER_COMMAND_FINISHIED,
	PLAYER_OBSERVER_COMMAND_ERROR,
	PLAYER_OBSERVER_COMMAND_PAUSED,
	PLAYER_OBSERVER_COMMAND_BUFFER_OVERRUN,
	PLAYER_OBSERVER_COMMAND_BUFFER_UNDERRUN,
	PLAYER_OBSERVER_COMMAND_BUFFER_UPDATED,
	PLAYER_OBSERVER_COMMAND_BUFFER_STATECHANGED,
} player_observer_command_t;

class MediaPlayerImpl : public std::enable_shared_from_this<MediaPlayerImpl>
{
public:
	MediaPlayerImpl(MediaPlayer &player);
	~MediaPlayerImpl();

	player_result_t create();
	player_result_t destroy();
	player_result_t prepare();
	player_result_t unprepare();
	player_result_t start();
	player_result_t pause();
	player_result_t stop();

	player_result_t getVolume(uint8_t *vol);
	player_result_t setVolume(uint8_t vol);

	player_result_t setDataSource(std::unique_ptr<stream::InputDataSource>);
	player_result_t setObserver(std::shared_ptr<MediaPlayerObserverInterface>);

	player_state_t getState();

	void notifySync();
	void notifyObserver(player_observer_command_t cmd, ...);

	void playback();

private:
	void createPlayer(player_result_t &ret);
	void destroyPlayer(player_result_t &ret);
	void preparePlayer(player_result_t &ret);
	void unpreparePlayer(player_result_t &ret);
	void startPlayer();
	void stopPlayer(player_result_t ret);
	void pausePlayer();
	void getPlayerVolume(uint8_t *vol, player_result_t &ret);
	void setPlayerVolume(uint8_t vol, player_result_t &ret);
	void setPlayerObserver(std::shared_ptr<MediaPlayerObserverInterface> observer);
	void setPlayerDataSource(std::shared_ptr<stream::InputDataSource> dataSource, player_result_t &ret);

private:
	MediaPlayer &mPlayer;
	std::atomic<player_state_t> mCurState;
	unsigned char *mBuffer;
	int mBufSize;
	std::mutex mCmdMtx;
	std::condition_variable mSyncCv;
	std::shared_ptr<MediaPlayerObserverInterface> mPlayerObserver;
	std::shared_ptr<stream::InputDataSource> mInputDataSource;
};
} // namespace media
#endif
