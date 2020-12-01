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

//void testImage::test_case1(){
//    // test download images
//    // verify the user information
//    QVERIFY(usr._email() == "test@test.com");
//    QString patientID = "123";
//    QString ctime = "2020-11-28";
//    imageInfo imagerequester(nullptr);
//    imagerequester.getImagesHttp(patientID, ctime);
//}

void testImage::test_case2(){
    // test getting ctime of a patient
    QString patientId = "123";
    imageInfo image_requester;
    image_requester.getCtimeHttp(patientId);
}

void testImage::test_case3(){
    // test upload images
    QString patientId = "123";
    QString ctime = "2020-12-01";
    QString filepath = "/Users/levius/Desktop/tempdata/123_2020-11-28/1.png";
    imageInfo image_requester;
    image_requester.uploadImageHttp(patientId, ctime, filepath);
}

void testImage::test_case4(){

}
