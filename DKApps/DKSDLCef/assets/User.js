DKCreate("DKWindow");
var url = "http://www.google.com";
var width = DKWindow_GetWidth();
var height = DKWindow_GetHeight();
DKCreate("DKCef,CefSDL,0,0,"+width+","+height+","+url);
var currentBrowser = DKCef_GetCurrentBrowser("CefSDL");
DKCef_SetUrl("CefSDL", url, currentBrowser);
DKCef_SetFocus("CefSDL");