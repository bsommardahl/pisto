open Util;

type viewing =
  | Tags
  | Products(string);

type state = {
  allProducts: list(Product.t),
  tags: list(string),
  customerName: string,
  orderId: int,
  isNew: bool,
  orderItems: list(OrderData.Order.orderItem),
  viewing,
};

type action =
  | Save
  | Pay
  | Delete
  | Close
  | SelectTag(string)
  | SelectProduct(Product.t)
  | DeselectTag;

let buildOrderItem = (product: Product.t) : OrderData.Order.orderItem => {
  product,
  addedOn: Js.Date.now(),
  salePrice: product.suggestedPrice,
};

let component = ReasonReact.reducerComponent("Order");

let make = _children => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SelectTag(tag) =>
      ReasonReact.Update({...state, viewing: Products(tag)})
    | DeselectTag => ReasonReact.Update({...state, viewing: Tags})
    | Save => ReasonReact.NoUpdate
    | Pay => ReasonReact.NoUpdate
    | Delete => ReasonReact.NoUpdate
    | Close => ReasonReact.NoUpdate
    | SelectProduct(product) =>
      ReasonReact.Update({
        ...state,
        orderItems:
          List.concat([state.orderItems, [buildOrderItem(product)]]),
      })
    },
  initialState: () => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let customerName =
      switch (Util.QueryParam.get("customerName", queryString)) {
      | Some(name) => name
      | None => "Cliente"
      };
    let orderId =
      switch (Util.QueryParam.get("orderId", queryString)) {
      | Some(id) => int_of_string(id)
      | None => 0
      };
    let isNew = orderId === 0;
    let products = Product.getProducts();
    {
      orderId,
      customerName,
      isNew,
      allProducts: products,
      tags: Product.getTags(products),
      viewing: Tags,
      orderItems: [],
    };
  },
  render: self => {
    let deselectTag = _event => self.send(DeselectTag);
    let selectTag = tag => self.send(SelectTag(tag));
    let selectProduct = product => self.send(SelectProduct(product));
    <div className="Order">
      <h1> (s("Order")) </h1>
      <h2> (s("Name:")) (s(self.state.customerName)) </h2>
      <h2> (s("Id:")) (s(string_of_int(self.state.orderId))) </h2>
      (
        switch (self.state.viewing) {
        | Tags =>
          self.state.tags
          |> List.map(tag => <div> <TagCard onSelect=selectTag tag /> </div>)
          |> Array.of_list
          |> ReasonReact.arrayToElement
        | Products(tag) =>
          Product.filterProducts(tag, self.state.allProducts)
          |> List.map(product =>
               <div>
                 <button onClick=deselectTag> (s("Atras")) </button>
                 <ProductCard onSelect=selectProduct product />
               </div>
             )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        }
      )
    </div>;
  },
};