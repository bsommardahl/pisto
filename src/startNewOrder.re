type action = 
  | NewOrder
  | Reset;

let component = ReasonReact.statelessComponent("StartNewOrder");

let goStartNewOrder = (_event) => Js.log("clicked");

let make = _children => {
  ...component,
  reducer: (action, state) => switch(action) {
    | NewOrder => NoneReasonReact.Router.url({ path: "/newOrder" });        
    | Reset => None
    },    
  render: _self =>
    let onClick = (_event) => _self.send(NewOrder);
    <div id="startNewOrder">
            <input
                _type="text"
                name="customerName"
                id="customerName"
                placeholder="Nombre del Cliente"
                required=Js.true_
            />
            <button onClick>
                (ReasonReact.stringToElement("Empezar Orden"))
            </button>
    </div>
    
};