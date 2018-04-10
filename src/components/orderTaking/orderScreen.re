open Util;

open Js.Promise;

type viewing =
  | Tags
  | Products(string);

type modifying =
  | Nothing
  | CustomerName;

type state = {
  allProducts: list(Product.t),
  tags: list(string),
  viewing,
  order: Order.orderVm,
  closedOrder: bool,
  modifying,
  allDiscounts: list(Discount.t),
};

type action =
  | SelectTag(string)
  | SelectProduct(Product.t)
  | DeselectTag
  | LoadOrder(Order.orderVm)
  | CloseOrderScreen
  | RemoveOrderItem(OrderItem.t)
  | ChangePaidDate(float)
  | ChangeCustomerName(string)
  | ProductsLoaded(list(Product.t))
  | DiscountsLoaded(list(Discount.t))
  | ApplyDiscount(Discount.t)
  | RemoveDiscount(Discount.t);

let buildOrderItem = (product: Product.t) : OrderItem.t => {
  sku: product.sku,
  name: product.name,
  suggestedPrice: product.suggestedPrice,
  addedOn: Js.Date.now(),
  salePrice: product.suggestedPrice,
  taxCalculation: product.taxCalculation,
};

let buildNewOrder = (customerName: string) : Order.orderVm => {
  id: None,
  customerName,
  orderItems: [],
  createdOn: Js.Date.now(),
  discounts: [],
  paid: None,
  lastUpdated: None,
  removed: false,
};

let getOrderVm = orderId =>
  OrderStore.get(orderId)
  |> Js.Promise.then_(order => {
       let vm = Order.vmFromExistingOrder(order);
       Js.Promise.resolve(vm);
     });

let dbUrl = "http://localhost:5984/orders";

let component = ReasonReact.reducerComponent("Order");

let make = (~goBack, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | ProductsLoaded(products) =>
      let tags = Product.getTags(products);
      ReasonReact.Update({...state, tags, allProducts: products});
    | DiscountsLoaded(discounts) =>
      ReasonReact.Update({...state, allDiscounts: discounts})
    | LoadOrder(order) =>
      ReasonReact.Update({
        ...state,
        order,
        closedOrder:
          switch (order.paid) {
          | Some(_) => true
          | None => false
          },
      })
    | ApplyDiscount(dis) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          discounts: List.concat([state.order.discounts, [dis]]),
        },
        allDiscounts:
          state.allDiscounts
          |> List.filter((d: Discount.t) => d.id !== dis.id),
      })
    | RemoveDiscount(dis) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          discounts:
            state.order.discounts
            |> List.filter((d: Discount.t) => d.id !== dis.id),
        },
        allDiscounts: List.concat([state.allDiscounts, [dis]]),
      })
    | ChangeCustomerName(name) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          customerName: name,
        },
      })
    | ChangePaidDate(date) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          paid:
            switch (state.order.paid) {
            | None => None
            | Some(paid) => Some({...paid, on: date})
            },
        },
      })
    | SelectTag(tag) =>
      ReasonReact.Update({...state, viewing: Products(tag)})
    | DeselectTag => ReasonReact.Update({...state, viewing: Tags})
    | CloseOrderScreen => ReasonReact.SideEffects((_self => goBack()))
    | RemoveOrderItem(orderItem) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          orderItems:
            state.order.orderItems |> List.filter(i => i !== orderItem),
        },
      })
    | SelectProduct(product) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          orderItems:
            List.concat([
              state.order.orderItems,
              [buildOrderItem(product)],
            ]),
        },
      })
    },
  initialState: () => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let customerName =
      switch (Util.QueryParam.get("customerName", queryString)) {
      | Some(name) => name
      | None => "Amado Cliente"
      };
    {
      closedOrder: false,
      allProducts: [],
      tags: [],
      viewing: Tags,
      order: buildNewOrder(customerName),
      modifying: Nothing,
      allDiscounts: [],
    };
  },
  didMount: self => {
    DiscountStore.getAll()
    |> then_(discounts => {
         self.send(DiscountsLoaded(discounts));
         resolve();
       })
    |> ignore;
    ProductStore.getAll()
    |> then_(prods => {
         self.send(ProductsLoaded(prods));
         resolve();
       })
    |> ignore;
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    switch (Util.QueryParam.get("orderId", queryString)) {
    | None => ReasonReact.NoUpdate
    | Some(orderId) =>
      getOrderVm(orderId)
      |> Js.Promise.then_(vm => {
           self.send(LoadOrder(vm));
           Js.Promise.resolve();
         })
      |> ignore;
      ReasonReact.NoUpdate;
    };
  },
  render: self => {
    Js.log("orderscreen:: " ++ self.state.order.customerName);
    let deselectTag = _event => self.send(DeselectTag);
    let selectTag = tag => self.send(SelectTag(tag));
    let selectProduct = product => self.send(SelectProduct(product));
    let discountSelected = discount => self.send(ApplyDiscount(discount));
    let discountDeselected = discount => self.send(RemoveDiscount(discount));
    <div className="order">
      <div className="order-header">
        <OrderActions
          closed=self.state.closedOrder
          order=self.state.order
          onFinish=((_) => self.send(CloseOrderScreen))
        />
        <div className="customer-name">
          <EditableText
            text=self.state.order.customerName
            onChange=(newName => self.send(ChangeCustomerName(newName)))
          />
        </div>
      </div>
      <div className="right-side">
        <OrderItems
          closed=self.state.closedOrder
          order=self.state.order
          deselectDiscount=discountDeselected
          onRemoveItem=(i => self.send(RemoveOrderItem(i)))
        />
      </div>
      <div className="left-side">
        (
          if (self.state.closedOrder) {
            <div>
              <h2> (s("Pagado")) </h2>
              <div className="paid-date">
                (
                  switch (self.state.order.paid) {
                  | None => <div />
                  | Some(paid) =>
                    <EditableDate
                      date=paid.on
                      onChange=(
                        newDate => self.send(ChangePaidDate(newDate))
                      )
                    />
                  }
                )
              </div>
            </div>;
          } else {
            <div>
              (
                switch (self.state.viewing) {
                | Tags =>
                  <div className="tags">
                    (
                      self.state.tags
                      |> List.map(tag => <TagCard onSelect=selectTag tag />)
                      |> Array.of_list
                      |> ReasonReact.arrayToElement
                    )
                  </div>
                | Products(tag) =>
                  <div className="products">
                    <div className="back-button-card card" onClick=deselectTag>
                      (s("Atras"))
                    </div>
                    (
                      Product.filterProducts(tag, self.state.allProducts)
                      |> List.map(product =>
                           <ProductCard onSelect=selectProduct product />
                         )
                      |> Array.of_list
                      |> ReasonReact.arrayToElement
                    )
                  </div>
                }
              )
              <div className="discounts">
                <DiscountSelector
                  discounts=self.state.allDiscounts
                  selected=discountSelected
                />
              </div>
            </div>;
          }
        )
      </div>
    </div>;
  },
};