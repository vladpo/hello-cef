#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_life_span_handler.h>
#include <include/cef_load_handler.h>
#include <include/wrapper/cef_helpers.h>
#include <iostream>
#include <fstream>

class ConsoleCefStringVisitor : public CefStringVisitor {
    public:
        void Visit(const CefString& content) {
            using namespace std;
            ofstream myfile;
            myfile.open ("/home/vpo/workspace/hello-cef/html-print/Release/out.txt");
            myfile << content.ToString();
            myfile.close();
        }

        IMPLEMENT_REFCOUNTING(ConsoleCefStringVisitor);
};

class ConsoleRenderHandler : public CefRenderHandler {
    public:
        bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
            std::cout << "GetViewRect()" << std::endl;
            rect = CefRect(0, 0, 500, 500);
            return true;
        }

        void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) {
            std::cout << "OnPaint()" << std::endl;
            browser->GetMainFrame()->GetSource(new ConsoleCefStringVisitor());
        }

        IMPLEMENT_REFCOUNTING(ConsoleRenderHandler);
};

class ConsoleBrowserClient : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
    public:
        ConsoleBrowserClient(CefRefPtr<ConsoleRenderHandler> renderHandler)
            : m_renderHandler(renderHandler) {;}
    
        virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler()
        {
            return this;
        }

        virtual CefRefPtr<CefLoadHandler> GetLoadHandler()
        {
            return this;
        }

        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
            return new ConsoleRenderHandler();
        }

    // CefLifeSpanHandler methods.
    void OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        // Must be executed on the UI thread.
       // CEF_REQUIRE_UI_THREAD();

        browser_id = browser->GetIdentifier();
    }

    bool DoClose(CefRefPtr<CefBrowser> browser)
    {
        // Must be executed on the UI thread.
        //CEF_REQUIRE_UI_THREAD();

        // Closing the main window requires special handling. See the DoClose()
        // documentation in the CEF header for a detailed description of this
        // process.
        if (browser->GetIdentifier() == browser_id)
        {
            // Set a flag to indicate that the window close should be allowed.
            closing = true;
        }

        // Allow the close. For windowed browsers this will result in the OS close
        // event being sent.
        return false;
    }

    void OnBeforeClose(CefRefPtr<CefBrowser> browser)
    {
    }

    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
    {
        std::cout << "OnLoadEnd(" << httpStatusCode << ")" << std::endl;
        loaded = true;
    }

    bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString & failedUrl, CefString & errorText)
    {
        std::cout << "OnLoadError()" << std::endl;
        loaded = true;
        return true;
    }

    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward)
    {
        std::cout << "OnLoadingStateChange()" << std::endl;
    }

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
    {
       std::cout << "OnLoadStart()" << std::endl;
    }

    bool closeAllowed() const
    {
        return closing;
    }

    bool isLoaded() const
    {
        return loaded;
    }

    private:
        int browser_id;
        bool closing = false;
        bool loaded = false;
        CefRefPtr<CefRenderHandler> m_renderHandler;

        IMPLEMENT_REFCOUNTING(ConsoleBrowserClient);
};

class MyRequestContextHandler :public CefRequestContextHandler
{
public:
    MyRequestContextHandler(){
        m_cookieMgr = CefCookieManager::CreateManager("/home/vpo/CefCookie", false, nullptr);
    };
    ~MyRequestContextHandler(){};
    CefRefPtr<CefCookieManager> GetCookieManager() override {
        return m_cookieMgr;
    }
private:
    CefRefPtr<CefCookieManager> m_cookieMgr;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(MyRequestContextHandler);
};

int main(int argc, char *argv[]) {
    CefMainArgs main_args(argc, argv);

    {
        int exit_code = CefExecuteProcess(main_args, NULL, NULL);
        // checkout CefApp, derive it and set it as second parameter, for more control on
        // command args and resources.
        if (exit_code >= 0) // child proccess has endend, so exit.
        {
            return exit_code;
        }
    }

    {
        CefSettings settings;
        settings.windowless_rendering_enabled = true;
        settings.multi_threaded_message_loop = true;
        settings.no_sandbox = true;
        bool result = CefInitialize(main_args, settings, nullptr, nullptr);
        if (!result) {
            return -1;
        }
    }

    ConsoleRenderHandler* crh;
    {
        crh = new ConsoleRenderHandler();
    }

    CefRefPtr<ConsoleBrowserClient> browserClient;
    CefRefPtr<CefRequestContext> rc;
    CefRequestContextSettings reqsettings;
    {
        CefWindowInfo window_info;
        window_info.SetAsWindowless(0);
        CefBrowserSettings browserSettings;
        CefWindowInfo SetAsWindowless;

        browserClient = new ConsoleBrowserClient(crh);
        rc = CefRequestContext::CreateContext(reqsettings, new MyRequestContextHandler());
        CefBrowserHost::CreateBrowser(window_info, browserClient.get(), "http://www.google.com", browserSettings, rc);
        CefDoMessageLoopWork();
    }

    {
        browserClient = nullptr;
        rc = nullptr;
        CefShutdown();
    }

    return 0;
}