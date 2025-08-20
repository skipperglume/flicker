#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>

typedef struct {
    uint32_t count;
    bool running;
} ClickerApp;

static void clicker_draw(Canvas* canvas, ClickerApp* app) {
    char buf[32];
    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 10, 20, "Clicker App");
    snprintf(buf, sizeof(buf), "Count: %lu", app->count);
    canvas_draw_str(canvas, 10, 40, buf);
}

static void clicker_input(InputEvent* event, void* context) {
    ClickerApp* app = context;
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyUp) {
            app->count++;
        } 
        else if(event->key == InputKeyDown) {
            app->count = 0;
        }
        else if(event->key == InputKeyBack) {
            app->running = false;
        }
    }
}

int32_t clicker_app(void* p) {
    UNUSED(p);
    ClickerApp app = {.count = 0, .running = true};
    Gui* gui = furi_record_open(RECORD_GUI);
    ViewPort* viewport = view_port_alloc();
    
    view_port_draw_callback_set(viewport, (ViewPortDrawCallback)clicker_draw, &app);
    view_port_input_callback_set(viewport, clicker_input, &app);
    // gui_add_view_port(gui, viewport, GuiLayerMax);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    while(app.running) {
        furi_delay_ms(50);
        // if(furi_should_exit()) break;
        view_port_update(viewport);
    }

    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_record_close(RECORD_GUI);
    return 0;
}
