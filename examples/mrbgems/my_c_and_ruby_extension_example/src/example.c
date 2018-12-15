#include <mruby.h>
#include <stdio.h>
#include "mruby/class.h"
#include "mruby/value.h"
#include "mruby/data.h"

struct example {
  int num;
};

const static struct mrb_data_type mrb_example_type = { "Example", mrb_free };

static mrb_value
mrb_example_init_copy(mrb_state *mrb, mrb_value copy)
{
  mrb_value src;

  mrb_get_args(mrb, "o", &src);
  if (mrb_obj_equal(mrb, copy, src)) return copy;
  if (!mrb_obj_is_instance_of(mrb, src, mrb_obj_class(mrb, copy))) {
    mrb_raise(mrb, E_TYPE_ERROR, "wrong argument class");
  }
  if (!DATA_PTR(copy)) {
    DATA_PTR(copy) = (struct example *)mrb_malloc(mrb, sizeof(struct example));
    DATA_TYPE(copy) = &mrb_example_type;
  }
  *(struct example *)DATA_PTR(copy) = *(struct example *)DATA_PTR(src);
  return copy;
}

static mrb_value
mrb_example_init(mrb_state *mrb, mrb_value self)
{
  struct example *h;

  h = (struct example *)mrb_malloc(mrb, sizeof(struct example));
  h->num = 1234;
  DATA_TYPE(self) = &mrb_example_type;
  DATA_PTR(self) = h;
  return self;
}

static mrb_value
mrb_example_get_method(mrb_state *mrb, mrb_value self)
{
  struct example *h;

  h = (struct example *)DATA_PTR(self);
  return mrb_fixnum_value(h->num);
}

static mrb_value
mrb_example_put_method(mrb_state *mrb, mrb_value self)
{
  struct example *h;
  mrb_int num;

  mrb_get_args(mrb, "i", &num);
  h = DATA_PTR(self);
  h->num = num;
  return mrb_fixnum_value(num);
}

void
mrb_c_and_ruby_extension_example_gem_init(mrb_state* mrb) {
  struct RClass *class_cextension;

  class_cextension = mrb_define_class(mrb, "Example", mrb->object_class);
  MRB_SET_INSTANCE_TT(class_cextension, MRB_TT_DATA);
  mrb_define_method(mrb, class_cextension, "initialize_copy", mrb_example_init_copy, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, class_cextension, "initialize", mrb_example_init, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_cextension, "get", mrb_example_get_method, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_cextension, "put", mrb_example_put_method, MRB_ARGS_REQ(1));
}

void
mrb_c_and_ruby_extension_example_gem_final(mrb_state* mrb) {
  /* finalizer */
}
