open Util;

type state = {customerName: string};

type action =
  | UpdateCustomerName(string);

let component = ReasonReact.reducerComponent("Home");

let make = (~onStartNewOrder, ~goToAdmin, _children) => {
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
        <div className="header-menu">
          <div
            className="card quiet-card"
            onClick=((_) => ReasonReact.Router.push("orders"))>
            (s("Ver Ordenes"))
          </div>
        </div>
        <input
          _type="text"
          className="customer-name-input"
          value=self.state.customerName
          name="customerName"
          onChange=(handleChange(self))
        />
        <div
          className="card"
          onClick=(_event => onStartNewOrder(self.state.customerName))>
          (s("Iniciar"))
        </div>
      </div>
      <OpenOrders />
    </div>;
  },
};