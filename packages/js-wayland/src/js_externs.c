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
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_shm *wl_shm;
};

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

void client_state_add_wl_surface (void* data, void* stuff_ptr) {
    struct client_state* state = data;
    struct wl_surface* surface_ptr = stuff_ptr;
    state->wl_surface = surface_ptr;
}
void client_state_add_xdg_surface (void* data, void* stuff_ptr) {
    struct client_state* state = data;
    struct xdg_surface* surface_ptr = stuff_ptr;
    state->xdg_surface = surface_ptr;
}

void client_state_add_xdg_toplevel (void* data, void* stuff_ptr) {
    struct client_state* state = data;
    struct xdg_toplevel* toplevel_ptr = stuff_ptr;
    state->xdg_toplevel = toplevel_ptr;
}
void* client_state_get_compositor_ptr (void* data) {
    struct client_state* state = data;
    return state->wl_compositor;
}
void* client_state_get_xdg_wm_base_ptr (void* data) {
    struct client_state* state = data;
    return state->xdg_wm_base;
}

// #region Implementations

struct wl_registry * wl_display_get_registry_impl(struct wl_display *display) {
   	struct wl_proxy *registry;

	registry = wl_proxy_marshal_flags((struct wl_proxy *) display,
			 WL_DISPLAY_GET_REGISTRY, &wl_registry_interface, wl_proxy_get_version((struct wl_proxy *) display), 0, NULL);

	return (struct wl_registry *) registry;
}

struct wl_surface * wl_compositor_create_surface_impl(
    struct wl_compositor *wl_compositor
) {
	struct wl_proxy *id;

	id = wl_proxy_marshal_flags((struct wl_proxy *) wl_compositor,
			 WL_COMPOSITOR_CREATE_SURFACE, &wl_surface_interface, wl_proxy_get_version((struct wl_proxy *) wl_compositor), 0, NULL);

	return (struct wl_surface *) id;
}

struct xdg_surface * xdg_wm_base_get_xdg_surface_impl(
    struct xdg_wm_base *xdg_wm_base,
    struct wl_surface *surface
) {
	struct wl_proxy *id;

	id = wl_proxy_marshal_flags((struct wl_proxy *) xdg_wm_base,
			 XDG_WM_BASE_GET_XDG_SURFACE, &xdg_surface_interface, wl_proxy_get_version((struct wl_proxy *) xdg_wm_base), 0, NULL, surface);

	return (struct xdg_surface *) id;
}

void xdg_toplevel_set_title_impl(
    struct xdg_toplevel *xdg_toplevel,
    const char *title
) {
	wl_proxy_marshal_flags((struct wl_proxy *) xdg_toplevel,
			 XDG_TOPLEVEL_SET_TITLE, NULL, wl_proxy_get_version((struct wl_proxy *) xdg_toplevel), 0, title);
}

void wl_surface_commit_impl(struct wl_surface* wl_surface) {
	wl_proxy_marshal_flags((struct wl_proxy *) wl_surface,
			 WL_SURFACE_COMMIT, NULL, wl_proxy_get_version((struct wl_proxy *) wl_surface), 0);
}

struct xdg_toplevel * xdg_surface_get_toplevel_impl(struct xdg_surface *xdg_surface) {
	struct wl_proxy *id;

	id = wl_proxy_marshal_flags((struct wl_proxy *) xdg_surface,
			 XDG_SURFACE_GET_TOPLEVEL, &xdg_toplevel_interface, wl_proxy_get_version((struct wl_proxy *) xdg_surface), 0, NULL);

	return (struct xdg_toplevel *) id;
}

// #endregion

// #region XDG BASE

static void xdg_wm_base_ping(
    void *data,
    struct xdg_wm_base *xdg_wm_base,
    uint32_t serial
) {
    printf("received xdg_wm_base ping\n");
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

// #endregion 

// #region Registry

void registry_global_handler (
    void *data,
    struct wl_registry *registry,
    uint32_t name,
    const char *interface,
    uint32_t version
) {
    printf("registry_global_handler: %s\n", interface);

    struct client_state *state = data;

    if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm = wl_registry_bind(
                registry, name, &wl_shm_interface, 1);
    } else if (strcmp(interface, wl_compositor_interface.name) == 0) {
    state->wl_compositor = wl_registry_bind(
            registry, name, &wl_compositor_interface, 4);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(
            registry,
            name,
            &xdg_wm_base_interface,
            1
        );
        xdg_wm_base_add_listener(
            state->xdg_wm_base,
            &xdg_wm_base_listener,
            state
        );
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

void attach_registry_listener(void *data) {
    struct client_state *state = data;

    wl_registry_add_listener(state->wl_registry, &registry_listener, data);
}
// #endregion

// #region SHM AND BUFFER

static void randname(char *buf) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A'+(r&15)+(r&16)*2;
        r >>= 5;
    }
}

static int create_shm_file(void) {
    int retries = 100;
    do {
        char name[] = "/wl_shm-XXXXXX";
        randname(name + sizeof(name) - 7);
        --retries;
        int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            shm_unlink(name);
            return fd;
        }
    } while (retries > 0 && errno == EEXIST);
    return -1;
}

static int allocate_shm_file(size_t size) {
    int fd = create_shm_file();
    if (fd < 0)
        return -1;
    int ret;
    do {
        ret = ftruncate(fd, size);
    } while (ret < 0 && errno == EINTR);
    if (ret < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static void wl_buffer_release(void *data, struct wl_buffer *wl_buffer) {
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

static struct wl_buffer * draw_frame(struct client_state *state) {
    const int width = 640, height = 480;
    int stride = width * 4;
    int size = stride * height;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = mmap(NULL, size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(state->wl_shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(
        pool,
        0,
        width,
        height,
        stride,
        WL_SHM_FORMAT_XRGB8888
    );
    wl_shm_pool_destroy(pool);
    close(fd);

    /* Draw checkerboxed background */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if ((x + y / 8 * 8) % 16 < 8)
                data[y * width + x] = 0xFF666666;
            else
                data[y * width + x] = 0xFFEEEEEE;
        }
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

// #endregion

// #region XDG_SURFACE

static void xdg_surface_configure(
    void *data,
    struct xdg_surface *xdg_surface, uint32_t serial
) {
    struct client_state *state = data;
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_buffer *buffer = draw_frame(state);
    wl_surface_attach(state->wl_surface, buffer, 0, 0);
    wl_surface_commit(state->wl_surface);
}

static const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

void attach_xdg_surface_listener(void *data) {
    struct client_state *state = data;

    xdg_surface_add_listener(state->xdg_surface, &xdg_surface_listener, data);
}

// #endregion

// #region Misc functions

void wrapped_roundtrip(void* client_state_ptr) {
    struct client_state* state = client_state_ptr;

    printf("js_extern.c: about to call wl_display_roundtrip\n");
    int result = wl_display_roundtrip(state->wl_display);
    printf("js_extern.c: wl_display_roundtrip returned %d\n", result);
}

// #endregion

int draw_window() {
    struct client_state state = { 0 };
    state.wl_display = wl_display_connect(NULL);
    state.wl_registry = wl_display_get_registry(state.wl_display);
    wl_registry_add_listener(state.wl_registry, &registry_listener, &state);

    // wait for the "initial" set of globals to appear
    wl_display_roundtrip(state.wl_display);

    state.wl_surface = wl_compositor_create_surface(state.wl_compositor);
    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base, state.wl_surface);
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);

    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_set_title(state.xdg_toplevel, "Example client");
    wl_surface_commit(state.wl_surface);

    while (wl_display_dispatch(state.wl_display)) {
        // nothing
    }
    return 0;
}