let component = ReasonReact.statelessComponent("Order");

let make = _children => {
  ...component,
  render: _self =>
    <div
      className="Order"
      /* <h1>{_self.props.customerName}</h1> */
    />,
};