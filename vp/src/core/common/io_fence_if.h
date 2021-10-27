#pragma once

struct io_fence_if {
    virtual ~io_fence_if() {}

    virtual void io_fence_done() = 0;

    virtual bool traced() = 0;
};