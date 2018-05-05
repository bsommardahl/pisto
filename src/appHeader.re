let component = ReasonReact.statelessComponent("AppHeader");

let make = _children => {
  ...component,
  render: _self =>
    <div className="App-header">
      <Loading />
      <div className="Title"> (ReactUtils.s("Pisto")) </div>
      <MenuOption messageKey="menu.openOrders" path="/" />
      <MenuOption messageKey="menu.closedOrders" path="orders" />
      <MenuOption messageKey="menu.expenses" path="expenses" />
      <MenuOption messageKey="menu.daily" path="daily" />
      <MenuOption messageKey="menu.admin" path="admin" />
    </div>,
};