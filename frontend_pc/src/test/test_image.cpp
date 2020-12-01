#include "test_image.h"

testImage::testImage()
{

}

testImage::~testImage(){

}

void testImage::initTestCase(){
    // first log in the user
    QString email = "test@test.com";
    QString pwd = "hello123";
    QVERIFY(usr.log_user_in(&requester, email, pwd));
    QVERIFY(usr._token() != "");
    // set the token of the patient class
    patient::set_token(usr._token());
    // set the token of the image class
    imageInfo::setToken(usr._token());
    imageInfo::setFilePath("/Users/levius/Desktop/tempdata");
}

void testImage::cleanupTestCase(){

}

void testImage::test_case1(){
    // verify the user information
    QVERIFY(usr._email() == "test@test.com");
    imageInfo image_requester;
    QString patientID = "123";
    QString ctime = "2020-11-28";
    image_requester.getImagesHttp(patientID, ctime);
}

void testImage::test_case2(){

}

void testImage::test_case3(){

}

void testImage::test_case4(){

}
