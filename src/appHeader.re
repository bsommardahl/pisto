let component = ReasonReact.statelessComponent("AppHeader");

let make = _children => {
  ...component,
  render: _self =>
    <div className="App-header">
      <Loading />
      <div className="Title"> (ReactUtils.s("Pisto")) </div>
    </div>,
};