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
      <div className="header">
        <input
          _type="text"
          className="customer-name-input"
          value=self.state.customerName
          name="customerName"
          onChange=(handleChange(self))
        />
        <Button
          local=true
          onClick=(_event => onStartNewOrder(self.state.customerName))
          label="order.startNew"
        />
      </div>
      <OpenOrders />
    </div>;
  },
};