type state = {
  customerName: string,
  showDialog: bool,
};

type action =
  | UpdateCustomerName(string)
  | ShowDialog
  | HideDialog;

let component = ReasonReact.reducerComponent("Home");

let make = (~onStartNewOrder, _children) => {
  ...component,
  initialState: () => {customerName: "", showDialog: false},
  reducer: (action, state) =>
    switch (action) {
    | UpdateCustomerName(customerName) =>
      ReasonReact.Update({...state, customerName})
    | ShowDialog => ReasonReact.Update({...state, showDialog: true})
    | HideDialog => ReasonReact.Update({...state, showDialog: false})
    },
  render: self => {
    let handleChange = ({ReasonReact.send}, event) => {
      let value = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;
      send(UpdateCustomerName(value));
    };
    <div className="home">
      <SearchModal
        label="modal.SearchProduct"
        isOpen=self.state.showDialog
        onConfirm=((_) => self.send(HideDialog))
        onCancel=((_) => self.send(HideDialog))
      />
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
        <Button
          local=true
          className="pay-button-card"
          label="order.searchProduct"
          onClick=((_) => self.send(ShowDialog))
        />
      </div>
      <OpenOrders />
    </div>;
  },
};