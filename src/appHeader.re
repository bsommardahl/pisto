let component = ReasonReact.statelessComponent("AppHeader");

let make = _children => {
  ...component,
  render: _self =>
    <div className="App-header">
      <Loading />
      <div className="Title"> (ReactUtils.s("Pisto")) </div>
      <MenuOption title="Inicio" path="/" />
      <MenuOption title="Ordenes" path="orders" />
      <MenuOption title="Gastos" path="expenses" />
      <MenuOption title="Diario" path="daily" />
      <MenuOption title="Admin" path="admin" />
    </div>,
};