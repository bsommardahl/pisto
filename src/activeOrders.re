let component = ReasonReact.statelessComponent("ActiveOrders");

/* Here I will have a list of order cards with minimal information. The user can select a card to
   view/edit the order. He will also be able to start a new order by entering the customer name. */
let make = _children => {
  ...component,
  render: _self => <div> <StartNewOrder /> </div>,
};