/*
 * Copyright 2012  Samsung Electronics Co., Ltd
 *
 * Licensed under the Flora License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.tizenopensource.org/license
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "mcfgwes.h"
#include "mcfutils.h"
#include "mcfresource.h"

#ifdef ENABLE_RESOURCE_DATA_FILE
#include <iostream>
#include <set>
#endif

#ifndef __MCF_RESOURCE_CACHE_H__
#define __MCF_RESOURCE_CACHE_H__



#ifdef ENABLE_RESOURCE_DATA_FILE

#pragma pack(1)
typedef struct _StoredMcfLayoutKeyProperties {
        mcfboolean fValid;
        mcfbyte labelCnt;
        mcfchar label[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_LABEL_FOR_ONE][RESOURCE_DATA_LABEL_LEN];
        mcfshort labelPropId;
        mcfchar labelImgPath[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX][RESOURCE_DATA_IMG_PATH_LEN];
        mcfchar bgImgPath[MCF_SHIFT_STATE_MAX][MCF_BUTTON_STATE_MAX][RESOURCE_DATA_IMG_PATH_LEN];
        mcfbyte keyValueCnt;
        mcfchar keyValue[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_MULTITAP_CHAR][RESOURCE_DATA_KEY_VALUE_LEN];
        mcfulong keyEvent[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_MULTITAP_CHAR];
        mcfchar longKeyValue[8];
        mcfulong longKeyEvent;
        mcfboolean useRepeatKey;
        mcfchar autopopupKeys[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_AUTOPOPUP_STRING][RESOURCE_DATA_AUTOPOPUP_KEY_LEN];
        mcfboolean dontClosePopup;
        mcfbyte extraOption;
        mcfbyte multitouchType;
        mcfbyte modifierDecorationId;
        mcfchar magnifierLabel[MCF_SHIFT_STATE_MAX][MAX_SIZE_OF_LABEL_FOR_ONE][RESOURCE_DATA_LABEL_LEN];
} StoredMcfLayoutKeyProperties;

typedef struct _StoredMcfLayoutKeyPropertiesHeader {
        mcfuint maxKeyset;
        mcfuint maxKey;
        mcfulong addrKey[MAX_KEYSET][MAX_KEY];
} StoredMcfLayoutKeyPropertiesHeader;
#pragma pack()


#endif


//MCF_BEGIN_DECLS

namespace mcf
{

typedef struct _McfButtonContext {
    mcfboolean used;
    mcfbyte multikeyIdx;
    MCFButtonState state;
} McfButtonContext;
/**
* @brief The class to store current state of key layout
*
* This class implements all functions for working as a soft-based keyboard
* In side of ISE developer, they can modify it by their requirements.
*/
class CMCFResourceCache
{
private:

#ifdef ENABLE_RESOURCE_DATA_FILE
    std::set<std::string> mLayoutKeyPropertiesStrings;
#endif

    static CMCFResourceCache* sInstance; /* For singleton */
    CMCFResourceCache();

#ifdef ENABLE_RESOURCE_DATA_FILE
    mcfchar* copy_string(const mcfchar *str);
    mcfint copy_from_stored(McfLayoutKeyProperties *prop, StoredMcfLayoutKeyProperties *stored);
    mcfboolean load_layoutkey_properties(mcfint keyset, McfLayoutKeyProperties (*pLayoutKeyProperties)[MAX_KEY]);
    mcfboolean load_layoutkey_properties(mcfint keyset, mcfint keyidx, McfLayoutKeyProperties *pLayoutKeyProperties);
#endif
public:
    ~CMCFResourceCache();

    static CMCFResourceCache* get_instance();

    mcfboolean recompute_layout(mcfwindow window);

    const McfLayout* get_cur_layout(mcfwindow window) const;
    McfLayoutKeyCoordination* get_cur_layout_key_coordination(mcfwindow window, mcfbyte keyindex);
    McfLayoutKeyConfigure* get_cur_layout_key_configure(mcfwindow window,mcfbyte keyindex);
    McfLayoutKeyProperties* get_cur_layout_key_properties(mcfwindow window,mcfbyte keyindex);

    const McfLabelProperties* get_label_properties(mcfshort labeltype, mcfbyte index) const;

    McfButtonContext* get_cur_button_context(mcfwindow window, mcfbyte keyindex);

    mcfboolean clear_private_keys();
    mcfint add_private_key(McfPrivateKeyProperties* properties, mcfboolean *fNeedInvaild);
    mcfboolean remove_private_key(mcfint id);

    mcfboolean copy_from_privatekeyproperties(const McfPrivateKeyProperties* privProperties,
    		                                  McfLayoutKeyProperties* properties,
    		                                  McfLayoutKeyConfigure *configure);
    mcfboolean copy_to_privatekeyproperties(const McfLayoutKeyProperties* properties,
    		                                const McfLayoutKeyConfigure *configure,
    		                                McfPrivateKeyProperties* privProperties);

    mcfboolean set_cur_themename(const mcfchar *themename);
    const mcfchar* get_cur_themename();

    mcfint set_private_key(McfPrivateKeyProperties* properties, mcfboolean fRedraw, mcfboolean fPendingUpdate);
    mcfint set_private_key(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx, mcfchar* label,
    		               mcfchar* imagelabel[MCF_BUTTON_STATE_MAX], mcfchar* imagebg[MCF_BUTTON_STATE_MAX],
    		               mcfulong keyEvent, mcfchar *keyValue, mcfboolean fRedraw, mcfboolean fPendingUpdate);
    void unset_private_key(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx);
    void unset_private_key(mcfint customID);

    mcfbyte find_keyidx_by_customid(mcfshort inputModeIdx, mcfbyte layoutIdx, mcfshort customID);
    void clone_keyproperties(McfPrivateKeyProperties* priv, mcfbyte inputModeIdx, mcfbyte layoutIdx, mcfbyte keyIdx);

    void enable_button(mcfbyte keyIdx);
    void disable_button(mcfbyte keyIdx);

private:
    mcfboolean resize_resource_elements_by_resolution();
    mcfboolean change_by_privatekey(const mcfbyte inputModeIdx, const mcfbyte layoutIdx, const mcfbyte keyIdx,
    		                        McfLayoutKeyProperties* properties, McfLayoutKeyConfigure* configure);

    void generate_autopopup_layout(const McfLayoutKeyProperties *properties,
                                   McfLayout *pCurLayout,
                                   McfLayoutKeyCoordination (*pCurLayoutKeyCoordination)[MAX_KEY],
                                   McfLayoutKeyConfigure (*pCurLayoutKeyConfigure)[MAX_KEY],
                                   McfLayoutKeyProperties (*pCurLayoutKeyProperties)[MAX_KEY],
                                   McfButtonContext (*pCurButtonContext)[MAX_KEY]);

protected:

    McfLayout mCurBaseLayout;
    McfLayoutKeyCoordination mCurBaseLayoutKeyCoordination[MAX_KEY];
    McfLayoutKeyConfigure mCurBaseLayoutKeyConfigure[MAX_KEY];
    McfLayoutKeyProperties mCurBaseLayoutKeyProperties[MAX_KEY];

    McfButtonContext mCurBaseButtonContext[MAX_KEY];

    McfLayout mCurPopupLayout[MAX_POPUP_WINDOW];
    McfLayoutKeyCoordination mCurPopupLayoutKeyCoordination[MAX_POPUP_WINDOW][MAX_KEY];
    McfLayoutKeyConfigure mCurPopupLayoutKeyConfigure[MAX_POPUP_WINDOW][MAX_KEY];
    McfLayoutKeyProperties mCurPopupLayoutKeyProperties[MAX_POPUP_WINDOW][MAX_KEY];

    McfButtonContext mCurPopupButtonContext[MAX_POPUP_WINDOW][MAX_KEY];

    McfPrivateKeyProperties mPrivateKeyProperties[MAX_PRIVATE_KEY];

    mcfchar mCurThemename[_POSIX_PATH_MAX];
};

}

//MCF_END_DECLS

#endif
