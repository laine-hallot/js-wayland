#include <wayland-client.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "js_externs.h"

struct client_state {
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_compositor *wl_compositor;
    struct xdg_wm_base *xdg_wm_base;
};

void registry_global_handler (
    void *data,
    struct wl_registry *registry,
    uint32_t name,
    const char *interface,
    uint32_t version
) {
    printf("registry_global_handler: %s\n", interface);

    struct client_state *state = data;
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
    state->wl_compositor = wl_registry_bind(
            registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(
            registry, name, &xdg_wm_base_interface, 1);
        /* xdg_wm_base_add_listener(state->xdg_wm_base,
                &xdg_wm_base_listener, state); */
    }
}


void registry_global_remove_handler(
    void *data,
    struct wl_registry *registry,
    uint32_t name
) {}

const struct wl_registry_listener registry_listener = {
    .global = registry_global_handler,
    .global_remove = registry_global_remove_handler
};

struct wl_registry * wl_display_get_registry_impl(struct wl_display *display) {
   	struct wl_proxy *registry;

	registry = wl_proxy_marshal_flags((struct wl_proxy *) display,
			 WL_DISPLAY_GET_REGISTRY, &wl_registry_interface, wl_proxy_get_version((struct wl_proxy *) display), 0, NULL);

	return (struct wl_registry *) registry;
}

void attach_registry_listener(void *data) {
    struct client_state *state = data;

    wl_registry_add_listener(state->wl_registry, &registry_listener, data);
}

struct client_state* create_heap_client_state () {
    struct client_state *state = malloc(sizeof(struct client_state));
    memset(state, 0, sizeof(struct client_state));
    return state;
}

void client_state_add_display (void* data, void* stuff_ptr) {
    struct client_state* state = data;
    struct wl_display* display_ptr = stuff_ptr;
    state->wl_display = display_ptr;
}

void client_state_add_registry (void* data, void* stuff_ptr) {
    struct client_state* state = data;
    struct wl_registry* registry_ptr = stuff_ptr;
    state->wl_registry = registry_ptr;
}

void wrapped_roundtrip(void* client_state_ptr) {
    struct client_state* state = client_state_ptr;

    printf("js_extern.c: about to call wl_display_roundtrip\n");
    int result = wl_display_roundtrip(state->wl_display);
    printf("js_extern.c: wl_display_roundtrip returned %d\n", result);
}

void* create_and_init_client_state(void) {
    struct client_state *state = malloc(sizeof(struct client_state));
    memset(state, 0, sizeof(struct client_state));

    state->wl_display = wl_display_connect(NULL);
    state->wl_registry = wl_display_get_registry_impl(state->wl_display);
    printf("js_extern.c: about to call wl_display_roundtrip\n");
    wl_registry_add_listener(state->wl_registry, &registry_listener, state);
    int result = wl_display_roundtrip(state->wl_display);
    printf("js_extern.c: wl_display_roundtrip returned %d\n", result);

    return state;
}