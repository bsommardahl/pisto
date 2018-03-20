
open OrderData;

type action =
  | SelectOrder(Order.order);

let openOrdersReducer = (action, _state) =>
  switch (action) {
  | SelectOrder(order) =>
    ReasonReact.SideEffects(
      (
        _self =>
          switch (order.id) {
          | Some(id) =>
            ReasonReact.Router.push("order?orderId=" ++ string_of_int(id))
          | None => ()
          }
      ),
    )
  };