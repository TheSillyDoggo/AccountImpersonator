#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class EditAccountPopup : public geode::Popup
{
    protected:
        TextInput* username;
        TextInput* accountID;
        TextInput* userID;
        TextInput* udid;

    public:
        CREATE_FUNC(EditAccountPopup);

        TextInput* createItem(CCNode* node, float& yPos, std::string text, std::string placeholder, std::string defaultS)
        {
            auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
            auto inp = TextInput::create(150, placeholder);
            inp->setString(defaultS);


            label->setScale(0.5f);

            float wid = label->getScaledContentWidth() + inp->getContentWidth() + 4 * 2;
            float x = (m_size.width - wid) / 2;

            label->setPosition(ccp(x, yPos));
            x += label->getScaledContentWidth() + 4;
            inp->setPosition(ccp(x + 4, yPos));
            yPos -= 35;

            label->setAnchorPoint(ccp(0, 0.5f));
            inp->setAnchorPoint(ccp(0, 0.5f));

            node->addChild(label);
            node->addChild(inp);
            return inp;
        }

        virtual bool init()
        {
            if (!Popup::init(ccp(350.f, 230.f), "GJ_square06.png"))
                return false;

            this->setTitle("Impersonate people!");
            m_closeBtn->setVisible(false);
            m_bgSprite->setColor(ccc3(80, 87, 105));
            auto spr2 = NineSlice::create("GJ_square07.png");
            spr2->setContentSize(m_size);
            spr2->setAnchorPoint(ccp(0, 0));
            m_mainLayer->addChild(spr2);

            float y = 173;
            username = createItem(m_mainLayer, y, "Username:", "Username", GJAccountManager::get()->m_username);
            accountID = createItem(m_mainLayer, y, "AccountID:", "ID", numToString<int>(GJAccountManager::get()->m_accountID));
            userID = createItem(m_mainLayer, y, "UserID:", "ID", numToString<int>(GameManager::get()->m_playerUserID));
            udid = createItem(m_mainLayer, y, "UDID:", "UDID", GameManager::get()->m_playerUDID);

            auto info = InfoAlertButton::create("UDID?", "Used for pretty much nothing, here for completenss sake", 0.5f);
            info->setPosition(udid->getPosition() + ccp(160, 0));
            m_buttonMenu->addChild(info);

            auto btn = ButtonSprite::create("Apply :3");
            auto btn2 = CCMenuItemSpriteExtra::create(btn, this, menu_selector(EditAccountPopup::onClose));
            m_buttonMenu->addChildAtPosition(btn2, Anchor::Bottom, ccp(0, 25));
            return true;
        }

        virtual void onClose(CCObject* sender)
        {
            GJAccountManager::get()->m_username = username->getString();
            GJAccountManager::get()->m_accountID = numFromString<int>(accountID->getString()).unwrap();
            GameManager::get()->m_playerUserID = numFromString<int>(userID->getString()).unwrap();
            GameManager::get()->m_playerUDID = udid->getString();

            Popup::onClose(sender);
        }
};

class $modify (MyMenuLayer, MenuLayer)
{
    void onEdit(CCObject* sender)
    {
        EditAccountPopup::create()->show();
    }

    CCNode* makeBtn()
    {
        auto spr = CCSprite::createWithSpriteFrameName("GJ_rateDiffBtn2_001.png");
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(MyMenuLayer::onEdit));
        return btn;
    }

    virtual bool init()
    {
        if (!MenuLayer::init())
            return false;

        auto menu = getChildByID("right-side-menu");

        menu->addChild(makeBtn());
        menu->updateLayout();
        return true;
    }
};