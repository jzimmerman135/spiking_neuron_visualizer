#include "encoder.h"
#include "assert.h"
#include "spikemath.h"



struct Encoder_T {
    float threshold;
    struct shape2_t shape;
    float2_t dv;
    float2_t v;
    bit2_t spikes;
};

Encoder_T Encoder_new(shape2_t shape)
{

    Encoder_T e = malloc(sizeof(struct Encoder_T));
    assert(e);

    struct shape2_t s = *shape;

    e->threshold = 1.f;
    e->shape = s;
    e->dv = float2_new(s.x, s.y);
    e->v = float2_new(s.x, s.y);
    e->spikes = bit2_new(s.x, s.y);

    return e;
}

void Encoder_free(Encoder_T *e)
{
    float2_free(&(*e)->dv);
    float2_free(&(*e)->v);
    bit2_free(&(*e)->spikes);
    free(*e);
}

/* change to alter nonlinear distrubution of spike frequency across [0, 1] */
static float distribution(float x);

void Encoder_set_current(Encoder_T e, float *currents)
{
    int size = e->shape.x * e->shape.y;
    float *dv = e->dv->data;

    for (int i = 0; i < size; i++)
        dv[i] = distribution(currents[i]);
}

bit2_t Encoder_spikes(Encoder_T e)
{
    /* increment by delta */
    float2_addto(e->v, e->dv);

    /* get spikes array */
    bit2_binset_f2_greater_than(e->spikes, e->v, e->threshold);

    /* don't remember why this has to be here */
    /* i think it preserved variation in encodings */
    float2_binsubtract(e->v, e->spikes);

    return e->spikes;
}

/*
    Distribution examples:
    ---------------------

    flat cubic:
    4 * (x * x * x) - 6 * (x * x) + 3 * (x)

    linear:
        x

    angled cubic: --nice
    3 * (x * x * x) - 4.5 * (x * x) + 2.5 * (x)
 */
static float distribution(float x)
{
    return 3 * (x * x * x) - 4.5 * (x * x) + 2.5 * (x);
}