type view =
  | Home
  | Order;

type customerName = string;

type state = {currentView: view};

type action =
  | ShowHome
  | ShowOrder;

let component = ReasonReact.reducerComponent("CafeRouter");

let joinStrings = l => l |> Array.of_list |> Js.Array.joinWith(",");

let make = _children => {
  ...component,
  initialState: () => {currentView: Home},
  reducer: (action, _state) =>
    switch (action) {
    | ShowHome => ReasonReact.Update({currentView: Home})
    | ShowOrder => ReasonReact.Update({currentView: Order})
    },
  subscriptions: self => [
    Sub(
      () =>
        ReasonReact.Router.watchUrl(url =>
          switch (url.path) {
          | [] => self.send(ShowHome)
          | ["order"] => self.send(ShowOrder)
          | p => Js.log("I don't know this path. " ++ (p |> joinStrings))
          }
        ),
      ReasonReact.Router.unwatchUrl,
    ),
  ],
  render: self => {
    let onStartNewOrder = customerName =>
      ReasonReact.Router.push("order?customerName=" ++ customerName);
    let finishedWithOrder = (_) => {
      Js.Console.log("Done with order.");
      self.send(ShowHome);
    };
    <div>
      (
        switch (self.state.currentView) {
        | Home => <Home onStartNewOrder />
        | Order => <Order finishedWithOrder />
        }
      )
    </div>;
  },
};