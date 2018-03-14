/* Not sure what's going on here with this build error*/
/* This router is where I will take the url and render the appropriate component */
type customerName = string;

type orderId = int;

type route =
  | Home
  | NewOrder(customerName)
  | Order(orderId);

type action =
  | ChangeRoute(route);

let reducer = (action, _state) =>
  switch (action) {
  | ChangeRoute(route) => ReasonReact.Update({route: route})
  };

let mapUrlToRoute = (url: ReasonReact.Router.url) =>
  switch (url.path) {
  | _ => Home
  /* | ["newOrder"] => NewOrder(url.search)
     | ["order", _] => Order(1) */
  };

let component = ReasonReact.reducerComponent("CafeRouter");

let make = _children => {
  ...component,
  reducer,
  initialState: () => {route: Home},
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          self.send(ChangeRoute(url |> mapUrlToRoute))
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: self =>
    <div>
      (
        switch (self.state.route) {
        | Home => <ActiveOrders />
        | NewOrder(_) => <Order isNew />
        | Order(_) => <Order orderId=1 />
        }
      )
    </div>,
};