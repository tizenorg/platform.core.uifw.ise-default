/*
 * Copyright 2012  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.tizenopensource.org/license
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



#include <stdio.h>
#include <stdlib.h>

#include "mcfresourcecache.h"
#include "mcfdebug.h"
#include "mcfcontext.h"

#include "mcfresourcekeys.h"
#include "mcfuibuilder.h"

using namespace mcf;

CMCFResourceCache* CMCFResourceCache::sInstance = NULL; /* For singleton */

CMCFResourceCache::CMCFResourceCache()
{
    MCF_DEBUG();
    resize_resource_elements_by_resolution();
    clear_private_keys();
    memset(mCurThemename, 0x00, sizeof(mCurThemename));
}

CMCFResourceCache::~CMCFResourceCache()
{
    MCF_DEBUG();
}

CMCFResourceCache*
CMCFResourceCache::get_instance()
{
    if (!sInstance) {
        sInstance = new CMCFResourceCache();
    }
    return (CMCFResourceCache*)sInstance;
}


#ifdef ENABLE_RESOURCE_DATA_FILE

mcfchar*
CMCFResourceCache::copy_string(const mcfchar *str)
{
    mcfchar *new_str;
    mcfint length;

    if (str) {
        std::string s(str);
        std::set<std::string>::iterator it =  mLayoutKeyPropertiesStrings.find(s);
        if (it != mLayoutKeyPropertiesStrings.end()) {
            s =* it;
            new_str = (mcfchar *)s.c_str();
        } else {
            length = strlen(str) + 1;
            new_str = (mcfchar*)malloc(length);
            if (new_str) {
                memcpy(new_str, str, length);
                new_str[length - 1] = '\0';
                s.assign(new_str);
                mLayoutKeyPropertiesStrings.insert(s);
            }
        }
    } else {
        new_str = NULL;
    }

    return new_str;
}


mcfint
CMCFResourceCache::copy_from_stored(McfLayoutKeyProperties *prop, StoredMcfLayoutKeyProperties *stored)
{
        int i,j;

        prop->fValid = stored->fValid;
        prop->labelCnt = stored->labelCnt;
        prop->labelPropId = stored->labelPropId;
        prop->keyValueCnt = stored->keyValueCnt;
        prop->longKeyEvent = stored->longKeyEvent;
        prop->useRepeatKey = stored->useRepeatKey;
        prop->dontClosePopup = stored->dontClosePopup;
        prop->extraOption = stored->extraOption;
        prop->multitouchType = stored->multitouchType;
        prop->modifierDecorationId = stored->modifierDecorationId;

        memcpy(prop->keyEvent, stored->keyEvent, sizeof(stored->keyEvent));

        prop->longKeyValue = copy_string((mcfchar *)stored->longKeyValue);

        for (i=0; i < MCF_SHIFT_STATE_MAX; i++) {
                for (j=0; j < MAX_SIZE_OF_LABEL_FOR_ONE; j++) {
                        if (stored->label[i][j][0]) {
                                prop->label[i][j] = copy_string((mcfchar *)stored->label[i][j]);
                        } else {
                                prop->label[i][j] = NULL;
                        }
                }

                for (j=0; j < MCF_BUTTON_STATE_MAX; j++) {
                        if (stored->labelImgPath[i][j][0]) {
                                prop->labelImgPath[i][j] = copy_string((mcfchar *)stored->labelImgPath[i][j]);
                        } else {
                                prop->labelImgPath[i][j] = NULL;
                        }

                        if (stored->bgImgPath[i][j][0]) {
                                prop->bgImgPath[i][j] = copy_string((mcfchar *)stored->bgImgPath[i][j]);
                        } else {
                                prop->bgImgPath[i][j] = NULL;
                        }
                }

                for (j=0; j < MAX_SIZE_OF_MULTITAP_CHAR; j++) {
                        if (stored->keyValue[i][j][0]) {
                                prop->keyValue[i][j] = copy_string((mcfchar *)stored->keyValue[i][j]);
                        } else {
                                prop->keyValue[i][j] = NULL;
                        }
                }

                for (j=0; j < MAX_SIZE_OF_AUTOPOPUP_STRING; j++) {
                        if (stored->autopopupKeys[i][j][0]) {
                                prop->autopopupKeys[i][j] = copy_string((mcfchar *)stored->autopopupKeys[i][j]);
                        } else {
                                prop->autopopupKeys[i][j] = NULL;
                        }
                }
        }

        return 0;
}



mcfboolean
CMCFResourceCache::load_layoutkey_properties(mcfint keyset, McfLayoutKeyProperties (*pLayoutKeyProperties)[MAX_KEY])
{
    FILE *fp;
    int i;

    StoredMcfLayoutKeyPropertiesHeader header;
    StoredMcfLayoutKeyProperties stored;

    fp = fopen(KEY_PROPERTIES_FILE, "r");
    if (!fp) {
        return FALSE;
    }

    fread(&header, sizeof(StoredMcfLayoutKeyPropertiesHeader), 1, fp);
    if (!header.addrKey[keyset][0]) {
            return FALSE;
    }

    fseek(fp, header.addrKey[keyset][0], SEEK_SET);
    memset((*pLayoutKeyProperties), 0x00, sizeof(McfLayoutKeyProperties)*MAX_KEY);
    for (i=0; i < MAX_KEY; i++) {
            if (header.addrKey[keyset][i]) {
                    fseek(fp, header.addrKey[keyset][i], SEEK_SET);
                    fread(&stored, sizeof(StoredMcfLayoutKeyProperties), 1, fp);
                    copy_from_stored(&(*pLayoutKeyProperties)[i], &stored);
            } else {
                    break;
            }
    }

    fclose(fp);

    return TRUE;
}

mcfboolean
CMCFResourceCache::load_layoutkey_properties(mcfint keyset, mcfint keyidx, McfLayoutKeyProperties *pLayoutKeyProperties)
{
    FILE *fp;
    int i;

    StoredMcfLayoutKeyPropertiesHeader header;
    StoredMcfLayoutKeyProperties stored;

    fp = fopen(KEY_PROPERTIES_FILE, "r");
    if (!fp) {
        return FALSE;
    }

    fread(&header, sizeof(StoredMcfLayoutKeyPropertiesHeader), 1, fp);
    if (!header.addrKey[keyset][keyidx]) {
            return FALSE;
    }

    memset(pLayoutKeyProperties, 0x00, sizeof(McfLayoutKeyProperties));

    fseek(fp, header.addrKey[keyset][keyidx], SEEK_SET);
    fread(&stored, sizeof(StoredMcfLayoutKeyProperties), 1, fp);
    copy_from_stored(pLayoutKeyProperties, &stored);

    fclose(fp);

    return TRUE;

}


#endif


/**
 * Returns the current layout data
 */
const McfLayout*
CMCFResourceCache::get_cur_layout(mcfwindow window) const
{
    MCF_DEBUG();

    const McfLayout *ret = NULL;
    CMCFWindows *windows = CMCFWindows::get_instance();

    if(windows) {
        if (windows->get_base_window() == window) {
            ret = &mCurBaseLayout;
        } else {
            mcfbyte popupindex = windows->find_popup_window_index(window);
            mcf_assert_return_false(popupindex < MAX_POPUP_WINDOW);
            if (popupindex < MAX_POPUP_WINDOW) {
                ret = &mCurPopupLayout[popupindex];
            }
        }
    }
    return ret;
}

/**
 * Translates the current x,y,width,height by the current screen resolution
 * This func should be called when the class init
 */
mcfboolean
CMCFResourceCache::resize_resource_elements_by_resolution()
{
    mcfint loop, innerLoop;
    CMCFUtils *utils = CMCFUtils::get_instance();

    mcfboolean invert_display = FALSE;
    if(MCF_AUTO_DETECT_PORTRAIT_LANDSCAPE) {
        mcfint x, y;
        utils->get_screen_resolution(&x, &y);
        /* If the width of screen is bigger than the height, switch portrait mode and landscape mode */
        if(x > y) {
            invert_display = TRUE;
        }
    }

    /* First we recaculate all the coordinations of each keys and sizes of layouts structure */
    for (loop = 0;loop < MAX_LAYOUT;loop++) {
        mcffloat scale_value_x, scale_value_y;
        if(invert_display) {
            if(mcf_layout[loop].displayType == MCFDISPLAY_PORTRAIT) {
                scale_value_x = utils->get_scale_rate_y();
                scale_value_y = utils->get_scale_rate_x();
            } else {
                scale_value_x = utils->get_scale_rate_x();
                scale_value_y = utils->get_scale_rate_y();
            }
        } else {
            if(mcf_layout[loop].displayType == MCFDISPLAY_PORTRAIT) {
                scale_value_x = utils->get_scale_rate_x();
                scale_value_y = utils->get_scale_rate_y();
            } else {
                scale_value_x = utils->get_scale_rate_y();
                scale_value_y = utils->get_scale_rate_x();
            }
        }
        mcf_layout[loop].width *= scale_value_x;
        mcf_layout[loop].height *= scale_value_y;
        for (innerLoop = 0;innerLoop < MAX_KEY;innerLoop++) {
            if (mcf_layout_key_coordination[loop][innerLoop].fValid == USED) {
                mcf_layout_key_coordination[loop][innerLoop].x *= scale_value_x;
                mcf_layout_key_coordination[loop][innerLoop].y *= scale_value_y;
                mcf_layout_key_coordination[loop][innerLoop].width *= scale_value_x;
                mcf_layout_key_coordination[loop][innerLoop].height *= scale_value_y;
                mcf_layout_key_coordination[loop][innerLoop].addHitLeft *= scale_value_x;
                mcf_layout_key_coordination[loop][innerLoop].addHitRight *= scale_value_x;
                mcf_layout_key_coordination[loop][innerLoop].addHitTop *= scale_value_y;
                mcf_layout_key_coordination[loop][innerLoop].addHitBottom *= scale_value_y;
                mcf_layout_key_coordination[loop][innerLoop].popXOffset *= scale_value_x;
                mcf_layout_key_coordination[loop][innerLoop].popYOffset *= scale_value_y;
                mcf_layout_key_coordination[loop][innerLoop].popImageX *= scale_value_x;
                mcf_layout_key_coordination[loop][innerLoop].popImageY *= scale_value_y;
            }
        }
    }

    /* And resize the font labels, adjusting the size of padding also */
    for (loop = 0;loop < MAX_LABEL_PROPERTIES;loop++) {
        for (innerLoop = 0;innerLoop < MAX_SIZE_OF_LABEL_FOR_ONE;innerLoop++) {
            if (mcf_key_label_properties[loop][innerLoop].fValid) {
                mcf_key_label_properties[loop][innerLoop].fontSize *= utils->get_smallest_scale_rate();
            }
            mcf_key_label_properties[loop][innerLoop].paddingX *= utils->get_smallest_scale_rate();
            mcf_key_label_properties[loop][innerLoop].paddingY *= utils->get_smallest_scale_rate();
            mcf_key_label_properties[loop][innerLoop].shadowDistance *= utils->get_smallest_scale_rate();
        }
    }

    /* Predefined font labels */
    CAND_LABEL_PROP.fontSize *= utils->get_smallest_scale_rate();
    CAND_LABEL_PROP.paddingX *= utils->get_smallest_scale_rate();
    CAND_LABEL_PROP.paddingY *= utils->get_smallest_scale_rate();

    ZOOMING_LABEL_PROP.fontSize *= utils->get_smallest_scale_rate();
    ZOOMING_LABEL_PROP.paddingX *= utils->get_smallest_scale_rate();
    ZOOMING_LABEL_PROP.paddingY *= utils->get_smallest_scale_rate();

    mcf_autopopup_configure.labelProp.fontSize *= utils->get_smallest_scale_rate();
    mcf_autopopup_configure.labelProp.paddingX *= utils->get_smallest_scale_rate();
    mcf_autopopup_configure.labelProp.paddingY *= utils->get_smallest_scale_rate();

    mcf_autopopup_configure.wndDecoSize *= utils->get_smallest_scale_rate();
    mcf_autopopup_configure.bgPadding *= utils->get_smallest_scale_rate();
    mcf_autopopup_configure.btnSpacing *= utils->get_smallest_scale_rate();
    utils->scale_x(&(mcf_autopopup_configure.btnWidth));
    utils->scale_y(&(mcf_autopopup_configure.btnHeight));

    if(MCF_AUTO_DETECT_PORTRAIT_LANDSCAPE) {
        mcfint x, y;
        utils->get_screen_resolution(&x, &y);
        /* If the width of screen is bigger than the height, switch portrait mode and landscape mode */
        if(x > y) {
            for (loop = 0;loop < MAX_INPUT_MODE;loop++) {
                mcfbyte temp = mcf_input_mode_configure[loop].layoutId[0];
                mcf_input_mode_configure[loop].layoutId[0] = mcf_input_mode_configure[loop].layoutId[1];
                mcf_input_mode_configure[loop].layoutId[1] = temp;
            }
            for (loop = 0;loop < MAX_INPUT_MODE_POPUP;loop++) {
                mcfbyte temp = mcf_input_mode_popup_configure[loop].layoutId[0];
                mcf_input_mode_popup_configure[loop].layoutId[0] = mcf_input_mode_popup_configure[loop].layoutId[1];
                mcf_input_mode_popup_configure[loop].layoutId[1] = temp;
            }
            for (loop = 0;loop < MODIFIER_DECORATION_NUM;loop++) {
                for(innerLoop = 0;innerLoop < KEY_MODIFIER_MAX;innerLoop++) {
                    mcfchar *temp;
                    temp = mcf_modifier_decoration[loop].bgImgPath[0][innerLoop];
                    mcf_modifier_decoration[loop].bgImgPath[0][innerLoop] = mcf_modifier_decoration[loop].bgImgPath[1][innerLoop];
                    mcf_modifier_decoration[loop].bgImgPath[1][innerLoop] = temp;
                }
            }
        }
    }
}

/**
 * Changes the current key properties by the current screen resolution
 * This func should be called when the class init
 */
mcfboolean
CMCFResourceCache::change_by_privatekey(const mcfbyte inputModeIdx, const mcfbyte layoutIdx, const mcfbyte keyIdx,
		                                McfLayoutKeyProperties* properties, McfLayoutKeyConfigure* configure)
{
    MCF_DEBUG();
    mcf_assert_return_false(properties);
    mcf_assert_return_false(configure);

    if(properties && configure) {
        for (int loop = 0;loop < MAX_PRIVATE_KEY; loop++) {
            if (mPrivateKeyProperties[loop].fValid == USED) {
                if (((inputModeIdx == mPrivateKeyProperties[loop].inputModeIdx ||
                	mPrivateKeyProperties[loop].inputModeIdx == -1)  &&
                    layoutIdx == mPrivateKeyProperties[loop].layoutIdx &&
                    keyIdx == mPrivateKeyProperties[loop].keyIdx) ||
                    (mPrivateKeyProperties[loop].inputModeIdx == -1 &&
                    mPrivateKeyProperties[loop].layoutIdx == -1 &&
                    configure->customID == mPrivateKeyProperties[loop].customID) ) {
                    /* sets the current properties to private key properties */
                      copy_from_privatekeyproperties(&mPrivateKeyProperties[loop], properties, configure);
                      break;
                }
            }
        }
    }

    return TRUE;
}

/**
 * Copys the given private properties data to the given key properties
 */
mcfboolean
CMCFResourceCache::copy_from_privatekeyproperties(const McfPrivateKeyProperties* privProperties,
		                                          McfLayoutKeyProperties* properties, McfLayoutKeyConfigure *configure)
{
    MCF_DEBUG();
    mcf_assert_return_false(privProperties);
    mcf_assert_return_false(properties);
    mcf_assert_return_false(configure);

    if(privProperties && configure && properties) {
        /* Configure */
        configure->customID = privProperties->customID;
        configure->buttonType = privProperties->buttonType;
        configure->keyType = privProperties->keyType;
        configure->popupType = privProperties->popupType;
        configure->fUseMagnifier = privProperties->fUseMagnifier;
        configure->fEnable = privProperties->fEnable;

        memcpy(configure->popupInputMode, privProperties->popupInputMode, sizeof(configure->popupInputMode));

        /* Properties */
        properties->labelCnt = privProperties->labelCnt;
        properties->labelPropId = privProperties->labelPropId;
        properties->keyValueCnt = privProperties->keyValueCnt;
        properties->longKeyValue = privProperties->longKeyValue;
        properties->longKeyEvent = privProperties->longKeyEvent;
        properties->useRepeatKey = privProperties->useRepeatKey;
        properties->dontClosePopup = privProperties->dontClosePopup;
        properties->extraOption = privProperties->extraOption;

        memcpy(properties->label, privProperties->label, sizeof(properties->label));
        memcpy(properties->labelImgPath, privProperties->labelImgPath, sizeof(properties->labelImgPath));
        memcpy(properties->bgImgPath, privProperties->bgImgPath, sizeof(properties->bgImgPath));
        memcpy(properties->keyValue, privProperties->keyValue, sizeof(properties->keyValue));
        memcpy(properties->keyEvent, privProperties->keyEvent, sizeof(properties->keyEvent));
        memcpy(properties->autopopupKeys, privProperties->autopopupKeys, sizeof(properties->autopopupKeys));
    }

    return TRUE;
}

/**
 * Copys the given properties data to the given private key properties
 */
mcfboolean
CMCFResourceCache::copy_to_privatekeyproperties(const McfLayoutKeyProperties* properties,
		                                        const McfLayoutKeyConfigure *configure, McfPrivateKeyProperties* privProperties)
{
    MCF_DEBUG();
    mcf_assert_return_false(privProperties);
    mcf_assert_return_false(properties);
	mcf_assert_return_false(configure);

    /* sets the current key Properties to private key privProperties */

    if(privProperties && configure && properties) {
        /* Configure */
        privProperties->customID = configure->customID;
        privProperties->buttonType = configure->buttonType;
        privProperties->keyType = configure->keyType;
        privProperties->popupType = configure->popupType;
        privProperties->fUseMagnifier = configure->fUseMagnifier;
        privProperties->fEnable = configure->fEnable;

        memcpy(privProperties->popupInputMode, configure->popupInputMode, sizeof(privProperties->popupInputMode));

        /* Properties */
        privProperties->labelCnt = properties->labelCnt;
        privProperties->labelPropId = properties->labelPropId;
        privProperties->keyValueCnt = properties->keyValueCnt;
        privProperties->longKeyValue = properties->longKeyValue;
        privProperties->longKeyEvent = properties->longKeyEvent;
        privProperties->useRepeatKey = properties->useRepeatKey;
        privProperties->dontClosePopup = properties->dontClosePopup;
        privProperties->extraOption = properties->extraOption;

        memcpy(privProperties->label, properties->label, sizeof(privProperties->label));
        memcpy(privProperties->labelImgPath, properties->labelImgPath, sizeof(privProperties->labelImgPath));
        memcpy(privProperties->bgImgPath, properties->bgImgPath, sizeof(privProperties->bgImgPath));
        memcpy(privProperties->keyValue, properties->keyValue, sizeof(privProperties->keyValue));
        memcpy(privProperties->keyEvent, properties->keyEvent, sizeof(privProperties->keyEvent));
        memcpy(privProperties->autopopupKeys, properties->autopopupKeys, sizeof(privProperties->autopopupKeys));
    }

    return TRUE;
}


/**
 * Adds a new private key
 * It will update it to the current cache properties context because the application can call it anytime
 * For adapting it in realtime, you should explictly call the draw_button function.
 *
 * @param[out] fNeedInvalid It will return true if the current private can be adapt into the current display.
 * @return id an array index of the private key
 */
mcfint
CMCFResourceCache::add_private_key(McfPrivateKeyProperties* privProperties, mcfboolean *fNeedInvaild)
{
    MCF_DEBUG();
    mcf_assert_return_false(privProperties);
    mcfint ret = NOT_USED;
    *fNeedInvaild = FALSE; /* don't need to update now */

    if(!mcf_check_arrindex(privProperties->keyIdx, MAX_KEY))
        return -1;

    /* Finds an index to be set */
    mcfint loop = 0;
    for (loop = 0;loop < MAX_PRIVATE_KEY; loop++) {
        if ((mPrivateKeyProperties[loop].inputModeIdx == privProperties->inputModeIdx) &&
            (mPrivateKeyProperties[loop].layoutIdx == privProperties->layoutIdx) &&
            mPrivateKeyProperties[loop].keyIdx == privProperties->keyIdx) {
            break;
        } else if ((mPrivateKeyProperties[loop].inputModeIdx == privProperties->inputModeIdx) &&
                   (mPrivateKeyProperties[loop].layoutIdx == privProperties->layoutIdx) &&
                   (privProperties->inputModeIdx == -1 && privProperties->layoutIdx == -1) &&
                   mPrivateKeyProperties[loop].customID == privProperties->customID) {
            break;
        } else if (mPrivateKeyProperties[loop].fValid == FALSE) {
        	break;
        }
    }

    if (loop == MAX_PRIVATE_KEY) {
        printf("Out of buffer!! could not insert new private data into buffer \n");
        return -1;
    }

    memcpy(&mPrivateKeyProperties[loop], privProperties, sizeof(McfPrivateKeyProperties));
    mPrivateKeyProperties[loop].fValid = USED;
    ret = loop;

    /* Checks whether it's the same with the current context */
    CMCFContext *context = CMCFContext::get_instance();

    /* For baselayout */
    mcfbyte layout = context->get_base_layout();
    mcfbyte inputmode = context->get_input_mode();
    if ((privProperties->inputModeIdx == inputmode && privProperties->layoutIdx == layout) ||
		(privProperties->inputModeIdx == -1 && privProperties->layoutIdx == -1 &&
		privProperties->customID == mCurBaseLayoutKeyConfigure[privProperties->keyIdx].customID)) {
        /* sets the current properties to private key properties */
        copy_from_privatekeyproperties(privProperties,
                                       &mCurBaseLayoutKeyProperties[privProperties->keyIdx],
                                       &mCurBaseLayoutKeyConfigure[privProperties->keyIdx]);
        *fNeedInvaild = TRUE;
        return ret;
    }

    if (ret == NOT_USED) {
        printf("Failed!. Out of private data buffer\n");
    }
    return ret;
}

/**
 * Removes the private data of the given id from McfPrivateKeyProperties buffer
 */
mcfboolean
CMCFResourceCache::remove_private_key(mcfint id)
{
    MCF_DEBUG();
    mcfint loop;
    CMCFContext *context = CMCFContext::get_instance();


    /* resets the current properties to predefined properties */
    mcfbyte keyidx = mPrivateKeyProperties[id].keyIdx;
    clone_keyproperties(&(mPrivateKeyProperties[id]),
                        mPrivateKeyProperties[id].inputModeIdx,
                        mPrivateKeyProperties[id].layoutIdx,
                        keyidx);
    copy_from_privatekeyproperties(&(mPrivateKeyProperties[id]),
                                   &mCurBaseLayoutKeyProperties[keyidx],
                                   &mCurBaseLayoutKeyConfigure[keyidx]);

    /* Shift all the privatekey properties to the left by 1, starting from the item next to the id th element */
    for (loop = id;loop < MAX_PRIVATE_KEY - 1; loop++) {
        memcpy(&mPrivateKeyProperties[loop], &mPrivateKeyProperties[loop + 1], sizeof(&mPrivateKeyProperties[loop]));
    }
    /* Fill 0x00 to the last element */
    memset(&mPrivateKeyProperties[MAX_PRIVATE_KEY - 1], 0x00, sizeof(McfPrivateKeyProperties));
    return TRUE;
}

/**
 * Clears all private keys
 */
mcfboolean
CMCFResourceCache::clear_private_keys()
{
    MCF_DEBUG();
    memset(mPrivateKeyProperties, 0x00, sizeof(McfPrivateKeyProperties) * MAX_PRIVATE_KEY);
    return TRUE;
}

/**
 * Re-computes the cache data of the given window. The cache data has been including the current key properties, button context, layout etc,,
 * Another role of this func is to adjust the current coordination according to the current resolution.
 * This func will be called when a newly window is created
 */
mcfboolean
CMCFResourceCache::recompute_layout(mcfwindow window)
{
    MCF_DEBUG();

    mcfint loop;

    CMCFWindows *windows = CMCFWindows::get_instance();
    CMCFContext *context = CMCFContext::get_instance();
    mcfbyte display = context->get_display();
    mcfbyte inputmode = context->get_input_mode();
    mcfint sublayoutidx = context->get_cur_sublayout_id();

    McfKeyset keyset = MAX_KEYSET;
    mcf8 popupindex = NOT_USED;

    McfLayout *pCurLayout = NULL;
    McfLayoutKeyCoordination (*pCurLayoutKeyCoordination)[MAX_KEY] = NULL;
    McfLayoutKeyConfigure (*pCurLayoutKeyConfigure)[MAX_KEY] = NULL;
    McfLayoutKeyProperties (*pCurLayoutKeyProperties)[MAX_KEY] = NULL;
    McfButtonContext (*pCurButtonContext)[MAX_KEY] = NULL;

    mcfshort layout =  NOT_USED;
    if(windows && context) {
        if (windows->is_base_window(window)) {
            layout = mcf_input_mode_configure[inputmode].layoutId[context->get_display()];

            pCurLayout = &mCurBaseLayout;
            pCurLayoutKeyCoordination = &mCurBaseLayoutKeyCoordination;
            pCurLayoutKeyConfigure = &mCurBaseLayoutKeyConfigure;
            pCurLayoutKeyProperties = &mCurBaseLayoutKeyProperties;
            pCurButtonContext = &mCurBaseButtonContext;

        } else {
            popupindex = windows->find_popup_window_index(window);
            /* Check if the popup index is in valid range */
            mcf_assert_return_false(popupindex >= 0 && popupindex < MAX_POPUP_WINDOW);

            if (popupindex >= 0 && popupindex < MAX_POPUP_WINDOW) {
                layout = context->get_popup_layout(window);

                pCurLayout = &mCurPopupLayout[popupindex];
                pCurLayoutKeyCoordination = &mCurPopupLayoutKeyCoordination[popupindex];
                pCurLayoutKeyConfigure = &mCurPopupLayoutKeyConfigure[popupindex];
                pCurLayoutKeyProperties = &mCurPopupLayoutKeyProperties[popupindex];

                pCurButtonContext = &mCurPopupButtonContext[popupindex];
            }
        }

        if (pCurLayout && pCurLayoutKeyCoordination && pCurLayoutKeyConfigure &&
        	pCurLayoutKeyProperties && pCurButtonContext) {
            /* If the layout index represents system-defined autopopup, generate layout and key properties data */
            if (layout == MCF_LAYOUT_AUTOPOPUP) {
                const McfLayoutKeyProperties *properties =
                    get_cur_layout_key_properties(context->get_cur_pressed_window(context->get_last_touch_device_id()),
                context->get_cur_pressed_key(context->get_last_touch_device_id()));

                generate_autopopup_layout(properties, pCurLayout, pCurLayoutKeyCoordination,
                                          pCurLayoutKeyConfigure, pCurLayoutKeyProperties, pCurButtonContext);
            } else {
                mcf_assert_return_false(layout >= 0 && layout < MAX_LAYOUT);

                if (mcf_check_arrindex(layout, MAX_LAYOUT)) {
                    memcpy(pCurLayout, &mcf_layout[layout], sizeof(McfLayout));

                    for (loop = 0;loop < (sizeof(mcf_layout_keyset) / sizeof(McfLayoutKeyset));loop++) {
                        if (mcf_layout_keyset[loop].inputmodeID == inputmode && mcf_layout_keyset[loop].layoutID == layout) {
                            keyset = static_cast<McfKeyset>(mcf_layout_keyset[loop].keysetID);
                            break;
                        }
                    }

                    mcf_assert_return_false(keyset >= 0 && keyset < MAX_KEYSET);

                    if (mcf_check_arrindex(keyset, MAX_KEYSET)) {
                        memcpy(pCurLayoutKeyCoordination, mcf_layout_key_coordination[layout],
                        	   sizeof(McfLayoutKeyCoordination) * MAX_KEY);
                        memcpy(pCurLayoutKeyConfigure, mcf_layout_key_configure[keyset],
                        	   sizeof(McfLayoutKeyConfigure) * MAX_KEY);
#ifdef ENABLE_RESOURCE_DATA_FILE
                        load_layoutkey_properties(keyset, pCurLayoutKeyProperties);
#else
                        memcpy(pCurLayoutKeyProperties, mcf_layout_key_properties[keyset],
                        	   sizeof(McfLayoutKeyProperties) * MAX_KEY);
#endif

                        memset(pCurButtonContext, 0x00, sizeof(McfButtonContext) * MAX_KEY);

                        for (loop = 0;loop < MAX_KEY;loop++) {
                            if (mcf_layout_key_configure[keyset][loop].fValid == USED) {
                                (*pCurButtonContext)[loop].used = TRUE;
                                if (popupindex != NOT_USED) {
                                    change_by_privatekey(inputmode, layout, loop, &(mCurPopupLayoutKeyProperties[popupindex][loop]),
                                                         &(mCurPopupLayoutKeyConfigure[popupindex][loop]));
                                } else {
                                    change_by_privatekey(inputmode, layout, loop, &(mCurBaseLayoutKeyProperties[loop]),
                                                         &(mCurBaseLayoutKeyConfigure[loop]));
                                }
                            }
                        }
                    }
                }
            }

            /* Resize window */
            if (windows->is_base_window(window)) {
                windows->resize_window(window, mCurBaseLayout.width, mCurBaseLayout.height);
                windows->resize_window(windows->get_dim_window(), mCurBaseLayout.width, mCurBaseLayout.height);
            }

            /* EFL testing */
            windows->update_window(window);
        }
    }

    return TRUE;
}

/**
 * Returns the current key_coordination data
 */
McfLayoutKeyCoordination*
CMCFResourceCache::get_cur_layout_key_coordination(mcfwindow window, mcfbyte keyindex)
{
    MCF_DEBUG();
    mcf_assert_return_null(keyindex < MAX_KEY);

    CMCFWindows *windows = CMCFWindows::get_instance();
    if (windows->get_base_window() == window) {
        if (keyindex < MAX_KEY) {
            return &mCurBaseLayoutKeyCoordination[keyindex];
        }
    } else {
        mcfbyte popupindex = windows->find_popup_window_index(window);
        mcf_assert_return_false(popupindex < MAX_POPUP_WINDOW);
        if (keyindex < MAX_KEY && popupindex < MAX_POPUP_WINDOW) {
            return &mCurPopupLayoutKeyCoordination[popupindex][keyindex];
        }
    }

    return NULL;
}

/**
 * Returns the current key_configure data
 */
McfLayoutKeyConfigure*
CMCFResourceCache::get_cur_layout_key_configure(mcfwindow window, mcfbyte keyindex)
{
    MCF_DEBUG();
    mcf_assert_return_null(keyindex < MAX_KEY);

    CMCFWindows *windows = CMCFWindows::get_instance();
    if (windows->get_base_window() == window) {
        if (keyindex < MAX_KEY) {
            return &mCurBaseLayoutKeyConfigure[keyindex];
        }
    } else {
        mcfbyte popupindex = windows->find_popup_window_index(window);
        mcf_assert_return_false(popupindex < MAX_POPUP_WINDOW);
        if (keyindex < MAX_KEY && popupindex < MAX_POPUP_WINDOW) {
            return &mCurPopupLayoutKeyConfigure[popupindex][keyindex];
        }
    }

    return NULL;
}

/**
 * Returns the current key_properties data
 */
McfLayoutKeyProperties*
CMCFResourceCache::get_cur_layout_key_properties(mcfwindow window, mcfbyte keyindex)
{
    MCF_DEBUG();
    mcf_assert_return_null(keyindex < MAX_KEY);

    CMCFWindows *windows = CMCFWindows::get_instance();
    if (windows->get_base_window() == window) {
        if (keyindex < MAX_KEY) {
            return &mCurBaseLayoutKeyProperties[keyindex];
        }
    } else {
        mcfbyte popupindex = windows->find_popup_window_index(window);
        mcf_assert_return_false(popupindex < MAX_POPUP_WINDOW);
        if (keyindex < MAX_KEY && popupindex < MAX_POPUP_WINDOW) {
            return &mCurPopupLayoutKeyProperties[popupindex][keyindex];
        }
    }

    return NULL;
}

/**
 * Returns the current label_properties data
 */
const McfLabelProperties*
CMCFResourceCache::get_label_properties(mcfshort labeltype, mcfbyte index) const
{
    MCF_DEBUG();

    if (labeltype == MCF_LABEL_PROPERTY_AUTOPOPUP) {
        return &mcf_autopopup_configure.labelProp;
    } else if (labeltype == MCF_LABEL_PROPERTY_CANDIDATE) {
        return &CAND_LABEL_PROP;
    } else if (labeltype == MCF_LABEL_PROPERTY_ZOOMING) {
        return &ZOOMING_LABEL_PROP;
    } else {
        if(mcf_check_arrindex(labeltype, MAX_LABEL_PROPERTIES) &&
           mcf_check_arrindex(index, MAX_SIZE_OF_LABEL_FOR_ONE)) {
          return &mcf_key_label_properties[labeltype][index];
        }
    }

    return NULL;
}

/**
 * Returns the current button_context data
 */
McfButtonContext*
CMCFResourceCache::get_cur_button_context(mcfwindow window, mcfbyte keyindex)
{
    MCF_DEBUG();
    mcf_assert_return_null(keyindex < MAX_KEY);

    CMCFWindows *windows = CMCFWindows::get_instance();
    if (windows->get_base_window() == window) {
        if (keyindex < MAX_KEY) {
            return &mCurBaseButtonContext[keyindex];
        }
    } else {
        mcfbyte popupindex = windows->find_popup_window_index(window);
        mcf_assert_return_null(popupindex < MAX_POPUP_WINDOW);
        if (keyindex < MAX_KEY && popupindex < MAX_POPUP_WINDOW) {
            return &mCurPopupButtonContext[popupindex][keyindex];
        }
    }

    return NULL;
}

/* Generate and fill autopopup layout data */
void CMCFResourceCache::generate_autopopup_layout(const McfLayoutKeyProperties *properties,
                                                  McfLayout *pCurLayout,
                                                  McfLayoutKeyCoordination (*pCurLayoutKeyCoordination)[MAX_KEY],
                                                  McfLayoutKeyConfigure (*pCurLayoutKeyConfigure)[MAX_KEY],
                                                  McfLayoutKeyProperties (*pCurLayoutKeyProperties)[MAX_KEY],
                                                  McfButtonContext (*pCurButtonContext)[MAX_KEY])
{
    MCF_DEBUG();

    CMCFUtils *utils = CMCFUtils::get_instance();
    CMCFContext *context = CMCFContext::get_instance();

    int loop;
    if (properties) {
        mcfbyte numKeys, numColumns, numRows;
        mcfint x, y, width, height;
        MCFShiftState shiftidx = context->get_shift_state();
        if(shiftidx < 0 || shiftidx >= MCF_SHIFT_STATE_MAX) shiftidx = MCF_SHIFT_STATE_OFF;
        if (utils->get_autopopup_window_variables(properties->autopopupKeys[shiftidx], &numKeys, &numColumns,
        		                                  &numRows, &width, &height)) {
            int row = 0, column = 0;

            pCurLayout->fValid = TRUE;
            pCurLayout->style = LAYOUT_STYLE_POPUP_GRAB;
            pCurLayout->name = NULL;
            pCurLayout->width = width;
            pCurLayout->height = height;
            memset(pCurLayout->imgPath, 0x00, sizeof(pCurLayout->imgPath));
            pCurLayout->fUseSWbg = TRUE;
            if (mcf_autopopup_configure.bgImgPath != NULL) {
                pCurLayout->imgPath[0] = mcf_autopopup_configure.bgImgPath;
                pCurLayout->fUseSWbg = FALSE;
            }
            pCurLayout->fUseSWBtn = TRUE;
            /* If button image path is set, use images instead of SW buttons */
            if (mcf_autopopup_configure.btnImgPath[0] != NULL) {
                pCurLayout->fUseSWBtn = FALSE;
            }
            pCurLayout->fUseZoomWnd = FALSE;
            pCurLayout->linkedLayoutId = NOT_USED;
            pCurLayout->fExtractBG = FALSE;
            pCurLayout->bgColor = mcf_autopopup_configure.bgColor;
            pCurLayout->bgLineWidth = mcf_autopopup_configure.bgLineWidth;
            pCurLayout->bgLineColor = mcf_autopopup_configure.bgLineColor;
            pCurLayout->addGrabLeft = mcf_autopopup_configure.addGrabLeft;
            pCurLayout->addGrabRight = mcf_autopopup_configure.addGrabRight;
            pCurLayout->addGrabTop = mcf_autopopup_configure.addGrabTop;
            pCurLayout->addGrabBottom = mcf_autopopup_configure.addGrabBottom;

            memset((*pCurLayoutKeyCoordination), 0x00, sizeof(McfLayoutKeyCoordination) * MAX_KEY);
            memset((*pCurLayoutKeyConfigure), 0x00, sizeof(McfLayoutKeyConfigure) * MAX_KEY);
            memset((*pCurLayoutKeyProperties), 0x00, sizeof(McfLayoutKeyProperties) * MAX_KEY);

            memset((*pCurButtonContext), 0x00, sizeof(McfButtonContext) * MAX_KEY);

            for (loop = 0;loop < (numColumns * numRows) && loop < MAX_KEY && loop < MAX_SIZE_OF_AUTOPOPUP_STRING;loop++) {
                char tempstr[2];
                column = (loop % numColumns);
                row = loop / numColumns;
                x = mcf_autopopup_configure.bgPadding + (mcf_autopopup_configure.btnWidth * column) +
                    (mcf_autopopup_configure.btnSpacing * column);
                y = mcf_autopopup_configure.bgPadding + (mcf_autopopup_configure.btnHeight * row) +
                    (mcf_autopopup_configure.btnSpacing * row);

                (*pCurLayoutKeyCoordination)[loop].fValid = TRUE;
                (*pCurLayoutKeyCoordination)[loop].x = x + utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                (*pCurLayoutKeyCoordination)[loop].y = y + utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                (*pCurLayoutKeyCoordination)[loop].width = mcf_autopopup_configure.btnWidth;
                (*pCurLayoutKeyCoordination)[loop].height = mcf_autopopup_configure.btnHeight;
                (*pCurLayoutKeyCoordination)[loop].popXOffset = 0;
                (*pCurLayoutKeyCoordination)[loop].popYOffset = 0;
                (*pCurLayoutKeyCoordination)[loop].popImageX = 0;
                (*pCurLayoutKeyCoordination)[loop].popImageY = 0;
                (*pCurLayoutKeyCoordination)[loop].subLayoutID = SUBLAYOUTID_NONE;

                (*pCurLayoutKeyConfigure)[loop].fValid = TRUE;
                if(loop < numKeys) {
                    (*pCurLayoutKeyConfigure)[loop].buttonType = BUTTON_TYPE_NORMAL;
                } else {
                    (*pCurLayoutKeyConfigure)[loop].buttonType = BUTTON_TYPE_UIITEM;
                }
                (*pCurLayoutKeyConfigure)[loop].keyType = KEY_TYPE_UNICODE;
                (*pCurLayoutKeyConfigure)[loop].popupType = POPUP_TYPE_NONE;
                (*pCurLayoutKeyConfigure)[loop].fUseMagnifier = FALSE;
                (*pCurLayoutKeyConfigure)[loop].fEnable = TRUE;
                memset((*pCurLayoutKeyConfigure)[loop].popupInputMode, NOT_USED,
                	   sizeof((*pCurLayoutKeyConfigure)[loop].popupInputMode));

                (*pCurLayoutKeyProperties)[loop].fValid = TRUE;
                (*pCurLayoutKeyProperties)[loop].labelCnt = 1;
                (*pCurLayoutKeyProperties)[loop].label[0][0] = properties->autopopupKeys[0][loop];
                (*pCurLayoutKeyProperties)[loop].label[1][0] = properties->autopopupKeys[1][loop];
                (*pCurLayoutKeyProperties)[loop].label[2][0] = properties->autopopupKeys[2][loop];
                (*pCurLayoutKeyProperties)[loop].labelPropId = MCF_LABEL_PROPERTY_AUTOPOPUP;
                memset((*pCurLayoutKeyProperties)[loop].labelImgPath, 0x00, sizeof((*pCurLayoutKeyProperties)[loop].labelImgPath));
                memset((*pCurLayoutKeyProperties)[loop].bgImgPath, 0x00, sizeof((*pCurLayoutKeyProperties)[loop].bgImgPath));
                for (int innerLoop = 0;innerLoop < MCF_BUTTON_STATE_MAX;innerLoop++) {
                    (*pCurLayoutKeyProperties)[loop].bgImgPath[0][innerLoop] =
                        (*pCurLayoutKeyProperties)[loop].bgImgPath[1][innerLoop] =
                            mcf_autopopup_configure.btnImgPath[innerLoop];
                }

                (*pCurLayoutKeyProperties)[loop].keyValueCnt = 1;
                (*pCurLayoutKeyProperties)[loop].keyValue[0][0] = properties->autopopupKeys[0][loop];
                (*pCurLayoutKeyProperties)[loop].keyValue[1][0] = properties->autopopupKeys[1][loop];
                (*pCurLayoutKeyProperties)[loop].keyValue[2][0] = properties->autopopupKeys[2][loop];
                (*pCurLayoutKeyProperties)[loop].keyEvent[0][0] = 0;
                (*pCurLayoutKeyProperties)[loop].keyEvent[1][0] = 0;
                (*pCurLayoutKeyProperties)[loop].longKeyValue = NULL;
                (*pCurLayoutKeyProperties)[loop].longKeyEvent = 0;

                (*pCurLayoutKeyProperties)[loop].autopopupKeys[0][0] = NULL;
                (*pCurLayoutKeyProperties)[loop].autopopupKeys[1][0] = NULL;
                (*pCurLayoutKeyProperties)[loop].autopopupKeys[2][0] = NULL;
                (*pCurLayoutKeyProperties)[loop].extraOption = 0;

                (*pCurButtonContext)[loop].used = TRUE;
            }
            if(mcf_autopopup_configure.wndDecoSize > 0) {
                mcfbyte decoidx;
                for (decoidx = 0;decoidx < MAX_WND_DECORATOR;decoidx++) {
                    if(loop + decoidx < MAX_KEY) {
                        (*pCurLayoutKeyCoordination)[loop + decoidx].fValid = TRUE;
                        switch(decoidx) {
                            case WND_DECORATOR_TOP_LEFT:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x = 0;
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y = 0;
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                            break;
                            case WND_DECORATOR_TOP_CENTER:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y = 0;
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    width - (2 * utils->get_scale_x(mcf_autopopup_configure.wndDecoSize));
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                break;
                            case WND_DECORATOR_TOP_RIGHT:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x =
                                    width - utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y = 0;
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                break;
                            case WND_DECORATOR_MIDDLE_LEFT:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x = 0;
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    height - (2 * utils->get_scale_y(mcf_autopopup_configure.wndDecoSize));
                                break;
                            case WND_DECORATOR_MIDDLE_RIGHT:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x =
                                    width - utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    height - (2 * utils->get_scale_y(mcf_autopopup_configure.wndDecoSize));
                                break;
                            case WND_DECORATOR_BOTTOM_LEFT:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x = 0;
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y =
                                    height - utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                break;
                            case WND_DECORATOR_BOTTOM_CENTER:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y =
                                    height - utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    width - (2 * utils->get_scale_x(mcf_autopopup_configure.wndDecoSize));
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                break;
                            case WND_DECORATOR_BOTTOM_RIGHT:
                                (*pCurLayoutKeyCoordination)[loop + decoidx].x =
                                    width - utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].y =
                                    height - utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].width =
                                    utils->get_scale_x(mcf_autopopup_configure.wndDecoSize);
                                (*pCurLayoutKeyCoordination)[loop + decoidx].height =
                                    utils->get_scale_y(mcf_autopopup_configure.wndDecoSize);
                                break;
                        }

                        (*pCurButtonContext)[loop + decoidx].used = TRUE;
                        (*pCurLayoutKeyConfigure)[loop + decoidx].fValid = TRUE;
                        (*pCurLayoutKeyConfigure)[loop + decoidx].buttonType = BUTTON_TYPE_UIITEM;
                        (*pCurLayoutKeyProperties)[loop + decoidx].fValid = TRUE;
                        (*pCurLayoutKeyProperties)[loop + decoidx].bgImgPath[0][0] =
                        (*pCurLayoutKeyProperties)[loop + decoidx].bgImgPath[0][1] =
                        (*pCurLayoutKeyProperties)[loop + decoidx].bgImgPath[1][0] =
                        (*pCurLayoutKeyProperties)[loop + decoidx].bgImgPath[1][1] =
                            mcf_autopopup_configure.wndDecoImgPath[decoidx];
                    }
                }
            }
        }
    }
}

/**
* Sets the current theme name
*/
mcfboolean
CMCFResourceCache::set_cur_themename( const mcfchar *themename )
{
    if (themename) {
        strncpy(mCurThemename, themename, _POSIX_PATH_MAX - 1);
        mCurThemename[_POSIX_PATH_MAX - 1] = '\0';

        char fontprop_path[_POSIX_PATH_MAX] = { 0 };
        CMCFUtils *utils = CMCFUtils::get_instance();
        utils->get_composed_path(fontprop_path, sizeof(fontprop_path), FONT_PROPERTIES_FILE);
        FILE *fp = fopen(fontprop_path, "r");
        if(fp) {
            mcfint index = 0;
            mcfint r,g,b,a;
            if(!feof(fp)) {
                for(mcfint shiftstate = 0;shiftstate < MCF_SHIFT_STATE_MAX && !feof(fp);shiftstate++) {
                    char comment[_POSIX_PATH_MAX];
                    for(mcfint buttonstate = 0;buttonstate < MCF_BUTTON_STATE_MAX && !feof(fp);buttonstate++) {
                        fscanf(fp, "%d%d%d%d", &r, &g, &b, &a);
                        mcf_autopopup_configure.labelProp.fontColor[shiftstate][buttonstate].r = r;
                        mcf_autopopup_configure.labelProp.fontColor[shiftstate][buttonstate].g = g;
                        mcf_autopopup_configure.labelProp.fontColor[shiftstate][buttonstate].b = b;
                        mcf_autopopup_configure.labelProp.fontColor[shiftstate][buttonstate].a = a;
                    }
                    fscanf(fp, "%s", comment);
                }
            }
            if(!feof(fp)) {
                mcfint subindex = 0;
            }
            index = 0;
            while(index < MAX_LABEL_PROPERTIES && !feof(fp)) {
                mcfint subindex = 0;
                while(subindex < MAX_SIZE_OF_LABEL_FOR_ONE && !feof(fp) &&
                	  mcf_key_label_properties[index][subindex].fValid) {
                    for(mcfint shiftstate = 0;shiftstate < MCF_SHIFT_STATE_MAX && !feof(fp);shiftstate++) {
                        char comment[_POSIX_PATH_MAX];
                        for(mcfint buttonstate = 0;buttonstate < MCF_BUTTON_STATE_MAX && !feof(fp);buttonstate++) {
                            fscanf(fp, "%d%d%d%d", &r, &g, &b, &a);
                            mcf_key_label_properties[index][subindex].fontColor[shiftstate][buttonstate].r = r;
                            mcf_key_label_properties[index][subindex].fontColor[shiftstate][buttonstate].g = g;
                            mcf_key_label_properties[index][subindex].fontColor[shiftstate][buttonstate].b = b;
                            mcf_key_label_properties[index][subindex].fontColor[shiftstate][buttonstate].a = a;
                        }
                        fscanf(fp, "%s", comment);
                    }
                    subindex++;
                }
                index++;
            }
        }
    }

    return TRUE;
}

const mcfchar*
CMCFResourceCache::get_cur_themename()
{
    return mCurThemename;
}

/**
 * Returns a template private key properties using key properties of the given context
 */
void
CMCFResourceCache::clone_keyproperties(McfPrivateKeyProperties* priv, mcfbyte inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx)
{
    MCF_DEBUG();
    if (priv) {
        CMCFResourceCache *cache = CMCFResourceCache::get_instance();
        memset(priv, 0x00, sizeof(McfPrivateKeyProperties));

        /* gets the value of the previous key properties */
        mcfint loop;
        McfKeyset keyset = MAX_KEYSET;
        McfLayoutKeyProperties keyProperties = { 0 };
        McfLayoutKeyConfigure keyConfigure = { 0 };
        for (loop = 0;loop < (int)((sizeof(mcf_layout_keyset) / sizeof(McfLayoutKeyset)));loop++)  {
            if (mcf_layout_keyset[loop].inputmodeID == inputModeIdx && mcf_layout_keyset[loop].layoutID == layoutIdx) {
                keyset = static_cast<McfKeyset>(mcf_layout_keyset[loop].keysetID);
                break;
            }
        }
        mcf_assert_return(keyset >= 0 && keyset < MAX_KEYSET);
        if (mcf_check_arrindex(keyset, MAX_KEYSET) && mcf_check_arrindex(keyIdx, MAX_KEY)) {
#ifdef ENABLE_RESOURCE_DATA_FILE
            load_layoutkey_properties(keyset, keyIdx, &keyProperties);
#else
            memcpy(&keyProperties, &mcf_layout_key_properties[keyset][keyIdx], sizeof(McfLayoutKeyProperties));
#endif
            memcpy(&keyConfigure, &mcf_layout_key_configure[keyset][keyIdx], sizeof(McfLayoutKeyConfigure));
        }

        mcf_assert_return(keyProperties.fValid == TRUE);

        /* Sets the default properties base on the properties values of the given context */
        cache->copy_to_privatekeyproperties(&keyProperties, &keyConfigure, priv);

        priv->inputModeIdx = inputModeIdx;
        priv->layoutIdx = layoutIdx;
        priv->keyIdx = keyIdx;
        priv->fValid = TRUE;
    }
}

/**
 * Sets a private key to the current context
 *
 * @Usage
 *       McfPrivateKeyProperties privProperties;
 *       gCore->clone_keyproperties(&privProperties, INPUT_MODE_NUMBER, LYT_PORTRAIT_NOW_3x4, 0);
 *       // change
 *       gCore->set_private_key(&privProperties, TRUE);
 */
mcfint
CMCFResourceCache::set_private_key(McfPrivateKeyProperties* properties, mcfboolean fRedraw, mcfboolean fPendingUpdate)
{
    MCF_DEBUG();
    CMCFContext *context = CMCFContext::get_instance();
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    mcfint privateId = NOT_USED;
    mcfboolean isNeedUpdate = FALSE;
    if(cache && context && properties) {
        privateId = cache->add_private_key(properties, &isNeedUpdate);
        if (fRedraw && isNeedUpdate && !fPendingUpdate) {
            CMCFUIBuilder *builder = CMCFUIBuilder::get_instance();
            CMCFWindows *windows = CMCFWindows::get_instance();
            if(builder && windows) {
                /* Fix me (we should consider popupwindow later)*/
                mcfwindow window = windows->get_base_window();
                /* Let's draw this private key only if the key's sublayout ID is active */
                McfLayoutKeyCoordination *coordination = get_cur_layout_key_coordination(window, properties->keyIdx);
                if(coordination) {
                    if(coordination->subLayoutID == SUBLAYOUTID_NONE ||
                       coordination->subLayoutID == context->get_cur_sublayout_id()) {
                        builder->draw_button(window, NULL, properties->keyIdx, mCurBaseButtonContext[properties->keyIdx].state);
                    }
                }
            }
        }
    }
    return privateId;
}

/**
 * Sets a private key to the current context
 * The other properties except given parameters will keep to the orginal value.
 * @Usage
 * gCore->set_private_key(INPUT_MODE_NUMBER, LYT_PORTRAIT_NOW_3x4, 0, "private", 999, TRUE);
 *
 * @param fRedraw If true, it will redraw the current key
 */
mcfint
CMCFResourceCache::set_private_key(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx, mcfchar* label,
		                           mcfchar* imagelabel[MCF_BUTTON_STATE_MAX], mcfchar* imagebg[MCF_BUTTON_STATE_MAX],
		                           mcfulong keyEvent, mcfchar *keyValue, mcfboolean fRedraw, mcfboolean fPendingUpdate)
{
    MCF_DEBUG();
    McfPrivateKeyProperties privProperties;
    CMCFResourceCache *cache = CMCFResourceCache::get_instance();
    memset(&privProperties, 0x00, sizeof(McfPrivateKeyProperties));

    /* If inputModeIx and layoutIdx both are NOT_USED, find current input mode and layout of base window */
    if (inputModeIdx == (mcfshort)NOT_USED && layoutIdx == (mcfbyte)NOT_USED) {
        CMCFContext *context = CMCFContext::get_instance();
        inputModeIdx = context->get_input_mode();
        layoutIdx = context->get_base_layout();
    }

    /* gets the value of the previous key properties */
    mcfint loop;
    McfKeyset keyset = MAX_KEYSET;
    McfLayoutKeyProperties keyProperties = { 0 };
    McfLayoutKeyConfigure keyConfigure = { 0 };
    for (loop = 0;loop < (int)((sizeof(mcf_layout_keyset) / sizeof(McfLayoutKeyset)));loop++) {
        if ((mcf_layout_keyset[loop].inputmodeID == inputModeIdx || inputModeIdx == -1) &&
            mcf_layout_keyset[loop].layoutID == layoutIdx) {
            keyset = static_cast<McfKeyset>(mcf_layout_keyset[loop].keysetID);
            break;
        }
    }
    mcf_assert_return_false(keyset >= 0 && keyset < MAX_KEYSET);
    if (mcf_check_arrindex(keyset, MAX_KEYSET) && mcf_check_arrindex(keyIdx, MAX_KEY)) {
#ifdef ENABLE_RESOURCE_DATA_FILE
            load_layoutkey_properties(keyset, keyIdx, &keyProperties);
#else
        memcpy(&keyProperties, &mcf_layout_key_properties[keyset][keyIdx], sizeof(McfLayoutKeyProperties));
#endif
        memcpy(&keyConfigure, &mcf_layout_key_configure[keyset][keyIdx], sizeof(McfLayoutKeyConfigure));
    }

    mcf_assert_return_false(keyProperties.fValid == TRUE);

    /* Sets the default properties base on the properties values of the given context */
    cache->copy_to_privatekeyproperties(&keyProperties, &keyConfigure, &privProperties);

    privProperties.fValid = TRUE;
    privProperties.inputModeIdx = inputModeIdx;
    privProperties.layoutIdx = layoutIdx;
    privProperties.keyIdx = keyIdx;
    privProperties.labelCnt = 1;
    privProperties.keyEvent[MCF_SHIFT_STATE_LOCK][0] = privProperties.keyEvent[MCF_SHIFT_STATE_ON][0] =
    privProperties.keyEvent[MCF_SHIFT_STATE_OFF][0] = keyEvent;
    privProperties.label[MCF_SHIFT_STATE_LOCK][0] = privProperties.label[MCF_SHIFT_STATE_ON][0] =
    privProperties.label[MCF_SHIFT_STATE_OFF][0] = label;
    privProperties.keyValue[MCF_SHIFT_STATE_LOCK][0] = privProperties.keyValue[MCF_SHIFT_STATE_ON][0] =
    privProperties.keyValue[MCF_SHIFT_STATE_OFF][0] = keyValue;
    for (loop =0;loop < MCF_BUTTON_STATE_MAX;loop++) {
        if (imagelabel) {
            privProperties.labelImgPath[MCF_SHIFT_STATE_LOCK][loop] =
            privProperties.labelImgPath[MCF_SHIFT_STATE_ON][loop] =
            privProperties.labelImgPath[MCF_SHIFT_STATE_OFF][loop] = imagelabel[loop];
        }
    }
    for (loop =0;loop < MCF_BUTTON_STATE_MAX;loop++) {
        if (imagebg) {
            privProperties.bgImgPath[MCF_SHIFT_STATE_LOCK][loop] =
            privProperties.bgImgPath[MCF_SHIFT_STATE_ON][loop] =
            privProperties.bgImgPath[MCF_SHIFT_STATE_OFF][loop] = imagebg[loop];
        }
    }
    return set_private_key(&privProperties, fRedraw, fPendingUpdate);
}

/**
* Unset private key for specific key
* @Usage
* gCore->unset_private_key(INPUT_MODE_NUMBER, LYT_PORTRAIT_NOW_3x4, 0);
*/
void
CMCFResourceCache::unset_private_key(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx)
{
    /* Since removed private key buffer will be shifted left, increase index only for successful deletion */
    mcfint loop = 0;
    mcfint count = 0;
    while (count++ < MAX_PRIVATE_KEY) {
        if ((mPrivateKeyProperties[loop].fValid &&
            mPrivateKeyProperties[loop].inputModeIdx == inputModeIdx) &&
            mPrivateKeyProperties[loop].layoutIdx == layoutIdx &&
            mPrivateKeyProperties[loop].keyIdx == keyIdx) {
                remove_private_key(loop);
        } else {
            loop++;
        }
    }
}

/**
* Unset private by customID, effective when removing all private keys with same customID
* @Usage
* gCore->unset_private_key(3);
*/
void
CMCFResourceCache::unset_private_key(mcfint customID)
{
    /* Since removed private key buffer will be shifted left, increase index only for successful deletion */
    mcfint loop = 0;
    mcfint count = 0;
    while (count++ < MAX_PRIVATE_KEY) {
        if(mPrivateKeyProperties[loop].fValid &&
            mPrivateKeyProperties[loop].customID == customID) {
            remove_private_key(loop);
        } else {
            loop++;
        }
    }
}

/**
* Find appropriate index of the key specified by customID
*/
mcfbyte
CMCFResourceCache::find_keyidx_by_customid(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfshort customID)
{
    mcfbyte ret = NOT_USED;
    mcfint loop;
    McfKeyset keyset = MAX_KEYSET;

    /* If inputModeIx and layoutIdx both are NOT_USED, find current input mode and layout of base window */
    if (inputModeIdx == (mcfshort)NOT_USED && layoutIdx == (mcfbyte)NOT_USED) {
        CMCFContext *context = CMCFContext::get_instance();
        inputModeIdx = context->get_input_mode();
        layoutIdx = context->get_base_layout();
    }

    for (loop = 0;loop < (sizeof(mcf_layout_keyset) / sizeof(McfLayoutKeyset));loop++) {
        if ((mcf_layout_keyset[loop].inputmodeID == inputModeIdx || inputModeIdx == -1) &&
        	mcf_layout_keyset[loop].layoutID == layoutIdx) {
            keyset = static_cast<McfKeyset>(mcf_layout_keyset[loop].keysetID);
            break;
        }
    }
    if (mcf_check_arrindex(keyset, MAX_KEYSET)) {
        for (loop = 0;loop < MAX_KEY;loop++) {
            if (mcf_layout_key_configure[keyset][loop].fValid != USED) break;
            if (mcf_layout_key_configure[keyset][loop].customID == customID) {
                ret = loop;
                break;
            }
        }
    }

    return ret;
}

/**
* Enable button for handling mouse events
*/
void CMCFResourceCache::enable_button(mcfbyte keyIdx)
{
    MCF_DEBUG();

    if (mcf_check_arrindex(keyIdx, MAX_KEY)) {
        mCurBaseButtonContext[keyIdx].state = BUTTON_STATE_NORMAL;

        CMCFWindows *windows = CMCFWindows::get_instance();
        /* Fix me (we should decide by which way we would redraw the button's region - direct or indirect?)*/
        windows->update_window(windows->get_base_window());
    }
}

/**
* Disable button to ignore mouse events
*/
void CMCFResourceCache::disable_button(mcfbyte keyIdx)
{
    MCF_DEBUG();

    if (mcf_check_arrindex(keyIdx, MAX_KEY)) {
        mCurBaseButtonContext[keyIdx].state = BUTTON_STATE_DISABLED;

        CMCFWindows *windows = CMCFWindows::get_instance();
        /* Fix me (we should decide by which way we would redraw the button's region - direct or indirect?)*/
        windows->update_window(windows->get_base_window());
    }
}
