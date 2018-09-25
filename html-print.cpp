#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>

class ConsoleCefStringVisitor : public CefStringVisitor {
    public:
        void Visit(const CefString& content) {
            ;
        }

        IMPLEMENT_REFCOUNTING(ConsoleCefStringVisitor);
};

class ConsoleRenderHandler : public CefRenderHandler {
    public:
        bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) {
            rect = CefRect(0, 0, 500, 500);
            return true;
        }

        void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) {
            browser->GetMainFrame()->GetSource(new ConsoleCefStringVisitor());
        }

        IMPLEMENT_REFCOUNTING(ConsoleRenderHandler);
};

class ConsoleBrowserClient : public CefClient {
    public:
        ConsoleBrowserClient(ConsoleRenderHandler *renderHandler)
            : m_renderHandler(renderHandler) {;}
    
        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() {
            return m_renderHandler;
        }

        CefRefPtr<CefRenderHandler> m_renderHandler;

        IMPLEMENT_REFCOUNTING(ConsoleBrowserClient);
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
        CefInitialize(main_args, settings, NULL, NULL);
    }

    ConsoleRenderHandler* crh;
    {
        crh = new ConsoleRenderHandler();
    }

    CefRefPtr<CefBrowser> browser;
    CefRefPtr<ConsoleBrowserClient> browserClient;
    {
        CefWindowInfo window_info;
        CefBrowserSettings browserSettings;

        window_info.SetAsWindowless(0);

        browserClient = new ConsoleBrowserClient(crh);
        browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "https://www.memboo.ro", browserSettings, nullptr);
    }

    {
        browser = nullptr;
        browserClient = nullptr;
        CefShutdown();
    }

    return 0;
}