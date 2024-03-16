#include "../dzy/http/http.h"
#include <iostream>
#include <fstream>
#include <string>
void test_http_req(){
    dzy::http::HttpRequest::ptr req(new dzy::http::HttpRequest(0x11));
    req->addHeader("Type", "text");
    req->setMethod(dzy::http::HttpMethod::GET);
    req->setPath("/home");
    std::string body = "i am dzyxxx";
    req->setBody(body);
    std::string str = req->toString();

    std::ofstream file;
    file.open("./data", std::ios::trunc);
    file.write(str.c_str(), str.size());
    std::cout<<"xxxx";
}


void test_http_res(){
    dzy::http::HttpResponse::ptr res(new dzy::http::HttpResponse(0x11,dzy::http::HttpStatus::OK,false));
    res->addHeader("Content/Type", "Text");
    std::string body = "i love you baby";
    res->setBody(body);

    std::cout<<res->toString();

}

int main(){
    //test_http_req();
    test_http_res();
    return 0;
}
