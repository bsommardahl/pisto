open Util;

type state = {customerName: string};

type action =
  | UpdateCustomerName(string);

let component = ReasonReact.reducerComponent("Home");

let make = (~onStartNewOrder, _children) => {
  ...component,
  initialState: () => {customerName: ""},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateCustomerName(customerName) =>
      ReasonReact.Update({customerName: customerName})
    },
  render: self => {
    let handleChange = ({ReasonReact.send}, event) => {
      let value = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;
      send(UpdateCustomerName(value));
    };
    <div className="home">
      <div className="start-new-order">
        <input
          _type="text"
          value=self.state.customerName
          name="customerName"
          onChange=(handleChange(self))
        />
        <button onClick=(_event => onStartNewOrder(self.state.customerName))>
          (s("Start Order"))
        </button>
      </div>
      <OpenOrders />
    </div>;
  },
};