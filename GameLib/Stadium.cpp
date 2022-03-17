/**
 * @file Stadium.cpp
 * @author SHOIMYA CHOWDHURY, Haoxiang Zhang
 */


#include "pch.h"
#include "Stadium.h"
#include <wx/graphics.h>
#include "Background.h"
#include "Platform.h"
#include "Door.h"
#include "Money.h"
#include "Wall.h"
#include "Snow.h"
#include "Stanley.h"
#include "Grass.h"
#include "VisitorDoor.h"
#include "VisitorMoney.h"
#include "Villain.h"

using namespace std;
const std::wstring ImagesDirectory = L"data/images";
const std::wstring LevelDirectory = L"data/levels";

/**
 * Generator
 */
Stadium::Stadium()
{
    double x = 100;
    double y = 100;


//    mGnome = make_shared<Gnome>(this);
//    mGnome->SetLocation(x,y);
    auto initialPicture = make_shared<Picture>(this);
    mMapPictures = {{L"i000",initialPicture}};
    mGameMode = begin;
    mLevel = make_shared<Level>(this);

}

/**
 * Draw the game
 * @param graphics The graphics context to draw on
 * @param width The width
 * @param height The height
 */
void Stadium::OnDraw(std::shared_ptr<wxGraphicsContext> graphics, double width, double height)
{
    ////     Automatic Scaling


///     Draw in virtual pixels on the graphics context

    mScale = double(height) / double(Height);
    graphics->Scale(mScale,mScale);

    auto virtualWidth = (double) width/mScale;
    graphics->PushState();

    auto xOffset = (double)-mGnome->GetX() + virtualWidth / 2.0f;


    // Compute the amount to scroll in the X dimension
    graphics->Translate(xOffset,0);
//    graphics->Scale(mScale,mScale);
    if (!mItems.empty())
    {
        for (auto obj : mItems)
        {
            if (!obj->GetPicture()->Empty())
            {
                    obj->Draw(graphics);
            }
        }
    }
    mGnome->Draw(graphics);
    graphics->PopState();
}

/**
 * Update item in game
 * @param elapsed The time to update
 */
void Stadium::Update(double elapsed)
{
    /*
    if(mGameMode == end)
    {
    }
    */
    for (auto item : mItems)
    {
        item->Update(elapsed);
    }
    mTime += elapsed;
//    mGnome->Update(elapsed);
}

/**
 * The collision test for item
 * @param item
 * @return
 */
std::shared_ptr<Item> Stadium::CollisionTest(Item *item)
{

    VisitorDoor visitorDoor;
    VisitorMoney visitorMoney;
    vector<shared_ptr<Item>> toRemove;
    for(auto i : mItems)
    {
        if(i->CollisionTest(item))
        {
            i->Accept(&visitorDoor);
            return i;
        }
    }
    return nullptr;
}

/**
 * Save the stadium as a .aqua XML file.
 *
 * Open an XML file and stream the stadium data to it.
 *
 * @param filename The filename of the file to save the stadium to
 */
void Stadium::Save(const wxString &filename)
{

}

void Stadium::Load(const wxString& filename)
{

    wxXmlDocument xmlDoc;
    if(!xmlDoc.Load(filename))
    {
        wxMessageBox(L"Unable to load xml file");
        return;
    }
    Clear();
    // Get the XML document root node
    auto root = xmlDoc.GetRoot();

    // return location of gnome
    auto vec = mLevel->XmlLoad(root);

    // set picture for gnome
    if(mMapPictures[L"i000"]->Empty())
    {
        mMapPictures[L"i000"]->SetImage(L"gnome.png");
        // create mGnome
    }
    mGnome = make_shared<Gnome>(this,mMapPictures[L"i000"]);
    mGnome->SetLocation(vec);
    mItems.push_back(mGnome);

//    bool mImageLoad = false;

    auto child = root->GetChildren();
    for( ; child; child=child->GetNext())
    {
       auto name = child->GetName();
       if(name == L"declarations")
       {
           auto subChild = child->GetChildren();
           for(;subChild;subChild=subChild->GetNext())
           {
               XmlPicture(subChild);
           }
       }
       else if(name == L"items")
       {
           auto subChild = child->GetChildren();
           for(;subChild;subChild=subChild->GetNext())
           {
               XmlItem(subChild);
           }
       }
    }

}

void Stadium::Clear()
{
    mItems.clear();
}

void Stadium::XmlItem(wxXmlNode* node)
{

    auto id= node->GetAttribute(L"id").ToStdWstring();

    // select type
    if (id == L"i001" || id == L"i002" )
    {
        auto picture = mMapPictures[id];
        // backgroundForest
        auto item = make_shared<Background>(this, picture);
        item->XmlLoad(node);
        AddItem(item);
    }
    else if (id == L"i003")
    {
        auto picture1 = mMapPictures[L"i0031"];
        auto picture2 = mMapPictures[L"i0032"];
        auto picture3 = mMapPictures[L"i0033"];
        auto item = make_shared<Grass>(this,picture1);
        auto item2 =  make_shared<Grass>(this,picture3);
        double pos = 0;
        node->GetAttribute(L"width").ToDouble(&pos);
        pos = pos / 32 - 2;
        pos = pos / 2;
        // [0] [1] [2] [3] [4] [5]
        // 1.5
        item->XmlLoad(node, -pos-0.5);
        AddItem(item);

        double i = -pos+0.5;
        while (i < pos+0.5) {
            auto item1 = make_shared<Grass>(this, picture2);
            item1->XmlLoad(node, i);
            AddItem(item1);
            i++;
        }
        item2->XmlLoad(node, pos+0.5);
        AddItem(item2);


    }

//        item = make_shared<Snow>(this,&picture);
    else if (id == L"i005" || id == L"i004")
    {
        // platform
        auto picture1 = mMapPictures[L"i0051"];
        auto picture2 = mMapPictures[L"i0052"];
        auto picture3 = mMapPictures[L"i0053"];
        if (id == L"i004")
        {
            picture1 = mMapPictures[L"i0041"];
            picture2 = mMapPictures[L"i0042"];
            picture3 = mMapPictures[L"i0043"];
        }
        auto item = make_shared<ItemPlatform>(this,picture1);
        auto item2 =  make_shared<ItemPlatform>(this,picture3);
        double pos = 0;
        node->GetAttribute(L"width").ToDouble(&pos);
        pos = pos / 32 - 2;
        pos = pos / 2;

            // [0] [1] [2] [3] [4] [5]
            // 1.5
        item->XmlLoad(node, -pos-0.5);
        AddItem(item);

        double i = -pos+0.5;
        while (i < pos+0.5) {
                auto item1 = make_shared<ItemPlatform>(this, picture2);
                item1->XmlLoad(node, i);
                AddItem(item1);
                i++;
        }
        item2->XmlLoad(node, pos+0.5);
        AddItem(item2);
    }
    else if (id == L"i006" || id == L"i007")
    {
        // wall1
        auto picture = mMapPictures[L"i006"];
        if (id == L"i007")
        {
            picture = mMapPictures[L"i007"];
        }
        double pos = 0;
        node->GetAttribute(L"height").ToDouble(&pos);
        pos = pos / 32;
        pos = pos / 2;
        double i = -pos+0.5;
        while (i<=pos-0.5)
        {
            auto item = make_shared<ItemPlatform>(this,picture);
            item->XmlLoady(node,i);
            AddItem(item);
            i++;
        }
    }
        // wall2
    else if (id == L"i008")
    {
        // money100
        auto picture = mMapPictures[L"i008"];
        auto item = make_shared<Money>(this, picture,100);
        item->XmlLoad(node);
        AddItem(item);
    }
    else if (id == L"i009")
    {
        // money 1000
        auto picture = mMapPictures[L"i009"];
        auto item = make_shared<Money>(this, picture,1000);
        item->XmlLoad(node);
        AddItem(item);
    }
    else if (id == L"i010")
    {
        // stanley
//        item = make_shared<Stanley>(this,&picture);

//        item = make_shared<Stanley>(this,&picture);

    }
    else if (id == L"i011")
    {
        // door
        auto picture = mMapPictures[L"i011"];
        auto item = make_shared<Door>(this,picture);
        item->XmlLoad(node);
        AddItem(item);

    }
    else if (id == L"i012")
    {
        // UofM
        auto picture = mMapPictures[L"i012"];
        auto item = make_shared<Villain>(this,picture);
        item->XmlLoad(node);
        AddItem(item);


    }
    else if (id == L"i013")
    {
        // wisc
        auto picture = mMapPictures[L"i013"];
        auto item = make_shared<Villain>(this,picture);
        item->XmlLoad(node);
        AddItem(item);
    }

/*    if(item != nullptr)
    {
        item->XmlLoad(node);
        AddItem(item);
    }
*/

}

void Stadium::XmlPicture(wxXmlNode* node)
{
    auto picture = make_shared<Picture>(this);
    auto id = node->GetAttribute(L"id").ToStdWstring();

    // select type
    if (id == L"i001" || id == L"i002")
    {
        auto imageName = node->GetAttribute(L"image").ToStdWstring();
        // backgroundForest
        picture->SetImage(imageName);
        mMapPictures[id] = picture;
    }
    else if (id == L"i003" || id == L"i005" || id == L"i004")
    {

        auto leftImage = node->GetAttribute(L"left-image").ToStdWstring();
        auto midImage =  node->GetAttribute(L"mid-image").ToStdWstring();
        auto rightImage =  node->GetAttribute(L"right-image").ToStdWstring();

        auto picture1 = make_shared<Picture>(this);
        auto picture2 = make_shared<Picture>(this);
        if(id == L"i003")
        {
            picture->SetImage(leftImage);
            mMapPictures[L"i0031"] = picture;

            picture1->SetImage(midImage);
            mMapPictures[L"i0032"] = picture1;

            picture2->SetImage(rightImage);
            mMapPictures[L"i0033"] = picture2;
        }
        else if (id == L"i005")
        {
            picture->SetImage(leftImage);
            mMapPictures[L"i0051"] = picture;

            picture1->SetImage(midImage);
            mMapPictures[L"i0052"] = picture1;

            picture2->SetImage(rightImage);
            mMapPictures[L"i0053"] = picture2;
        }
        else
        {
            picture->SetImage(leftImage);
            mMapPictures[L"i0041"] = picture;

            picture1->SetImage(midImage);
            mMapPictures[L"i0042"] = picture1;

            picture2->SetImage(rightImage);
            mMapPictures[L"i0043"] = picture2;

        }
        picture->SetImagePos(L"left");
        picture1->SetImagePos(L"mid");
        picture2->SetImagePos(L"right");
    }
//    else if (id == L"i005")
//    {
/*        Picture p1(this);
        Picture p2(this);
        Picture p3(this);
        leftImage = node->GetAttribute(L"left-image").ToStdWstring();
        midImage = node->GetAttribute(L"mid-image").ToStdWstring();
        rightImage = node->GetAttribute(L"right-image").ToStdWstring();
        p1.SetImage(leftImage);
        p2.SetImage(midImage);
        p3.SetImage(rightImage);
        */
//        mMapPictures[3] = &p1;
//        mMapPictures[4] = &p2;
//        mMapPictures[5] = &p3;
        // platform
//    }
   
    else if (id == L"i006" || id == L"i007")
    {
        // wall1
        auto imageName = node->GetAttribute(L"image").ToStdWstring();
        picture->SetImage(imageName);
        mMapPictures[id] = picture;

    }
        // wall2
         
    else if (id == L"i008")
    {
        // money100
        auto imageName = node->GetAttribute(L"image").ToStdWstring();
        picture->SetImage(imageName);
        mMapPictures[id] = picture;

    }
    else if (id == L"i009")
    {
        // money 1000
        auto imageName = node->GetAttribute(L"image").ToStdWstring();
        picture->SetImage(imageName);
        mMapPictures[id] = picture;

    }
    /*
    else if (id == L"i010")
    {
        // stanley
//        item = make_shared<Stanley>(this,&picture);

    }
     */
    else if (id == L"i011")
    {
        // door
        auto imageName = node->GetAttribute(L"image").ToStdWstring();
        picture->SetImage(imageName);
        mMapPictures[id] = picture;

    }

    else if (id == L"i012")
    {
        // UofM

      auto imageName = node->GetAttribute(L"image").ToStdWstring();
        picture->SetImage(imageName);
        mMapPictures[id] = picture;

    }
    else if (id == L"i013")
    {
        // wisc
        auto imageName = node->GetAttribute(L"image").ToStdWstring();
        picture->SetImage(imageName);
        mMapPictures[id] = picture;
    }
}

void Stadium::AddItem(const std::shared_ptr<Item>& item)
{
    mItems.push_back(item);
}

void Stadium::Load(int level)
{
//    mTime = 0;
    wstring path;
    switch (level){
        case 0:
            path = mLevel->Level0();
            break;

        case 1:
            path = mLevel->Level1();
            break;

        case 2:
            path = mLevel->Level2();
            break;

        case 3:
            path = mLevel->Level3();
            break;

    default:
        break;
    }
    if(level>3)
    {
        level = 3;
    }
    SetLevelNum(level);
    Load(path);
}

std::shared_ptr<Item> Stadium::HitTest(int x, int y)
{
    for (auto i = mItems.rbegin(); i!=mItems.rend(); i++) {
        if ((*i)->HitTest(x, y)) {
            return *i;
        }
    }
    return nullptr;
}

void Stadium::Delete(shared_ptr<Item> item)
{
    if(const auto loc = find(mItems.begin(),mItems.end(),item); loc != mItems.end())
    {
        mItems.erase(loc);
    }
}
