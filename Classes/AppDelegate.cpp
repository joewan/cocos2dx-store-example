/*
 * Copyright (C) 2012 Soomla Inc.
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
 */

#include "AppDelegate.h"
#include "CCStoreController.h"
#include "CCStoreInventory.h"
#include "CCSoomla.h"
#include "MuffinRushAssets.h"
#include "StoreAScene.h"
#include "MainScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
	handler = new EventHandler();
}

AppDelegate::~AppDelegate() 
{
	soomla::CCSoomla::sharedSoomla()->removeEventHandler(handler);
    delete handler;
}

bool AppDelegate::applicationDidFinishLaunching() {
	soomla::CCSoomla::sharedSoomla()->setSoomSec("ExampleSoomSecret");
	soomla::CCSoomla::sharedSoomla()->setAndroidPublicKey("ExamplePublicKey");
	soomla::CCSoomla::sharedSoomla()->setCustomSecret("ExampleCustomSecret");

	// Set Android Test Mode for debugging your store purchases on Android
    // REMOVE THIS BEFORE YOU PUBLISH YOUR GAME!
    soomla::CCSoomla::sharedSoomla()->setAndroidTestMode(true);

    // We initialize CCStoreController and the event handler before
    // we open the store.
    soomla::CCSoomla::sharedSoomla()->addEventHandler(handler);
	
	MuffinRushAssets *assets = MuffinRushAssets::create();
	soomla::CCStoreController::createShared(assets);

	/*
	 * ** Set the amount of each currency to 10,000 if the **
     * ** balance drops under 1,000                        **
	 *
	 * ** Of course, this is just for testing...           **
	 */

    CCArray *currencies = assets->getCurrencies();
	CCObject *currencyObject;
	CCARRAY_FOREACH(currencies, currencyObject) {
		soomla::CCVirtualCurrency *vc =
			dynamic_cast<soomla::CCVirtualCurrency *>(currencyObject);
		int balance = soomla::CCStoreInventory::sharedStoreInventory()->
			getItemBalance(vc->getItemId()->getCString(), NULL);
		if (balance < 1000) {
			soomla::CCStoreInventory::sharedStoreInventory()->
				giveItem(vc->getItemId()->getCString(), 10000 - balance, NULL);
		}
	}
	
	CCDirector::sharedDirector()->setOpenGLView(CCEGLView::sharedOpenGLView());
	// Disable FPS counter
	CCDirector::sharedDirector()->setDisplayStats(false);
	// Set frames per second
	CCDirector::sharedDirector()->setAnimationInterval(1.0/60);

	CCScene *scene = MainScene::getMainScene();
	CCDirector::sharedDirector()->runWithScene(scene);
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
}
