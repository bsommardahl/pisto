open Js.Promise;

open OrderHelper;

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
  sku: string,
  showDialog: bool,
  skuEnabled: bool,
};

type action =
  | SelectTag(string)
  | SelectProduct(Product.t)
  | DeselectTag
  | LoadOrder(Order.orderVm)
  | CloseOrderScreen
  | ChangePaidDate(float)
  | ChangeCustomerName(string)
  | ChangeOrder(Order.orderVm)
  | ProductsLoaded(list(Product.t))
  | DiscountsLoaded(list(Discount.t))
  | ApplyDiscount(Discount.t)
  /* | RemoveOrderItem(OrderItem.t)  */
  | RemoveDiscount(Discount.t)
  | ShowDialog
  | HideDialog
  | EnableSku
  | DisableSku;

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
    | ChangeOrder(order) => 
    ReasonReact.UpdateWithSideEffects({...state,order:{...state.order,orderItems:List.concat([state.order.orderItems])}}, _=>{  Js.log(state.order)})
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
       /* | RemoveOrderItem(orderItem) =>
      Js.log("here");
      ReasonReact.Update(
        {
          ...state,
          order: {
            ...state.order,
            orderItems:
              state.order.orderItems |> List.filter(i => i !== orderItem),
          },
        },
      ); */
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
    | SelectProduct(product) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          order: {
            ...state.order,
            orderItems:
              List.concat([
                state.order.orderItems,
                [buildOrderItem(product)],
              ]),
          },
        },
        (self => self.send(HideDialog)),
      )
    | ShowDialog =>
      ReasonReact.UpdateWithSideEffects(
        {...state, showDialog: true},
        (self => self.send(DisableSku)),
      )
    | HideDialog =>
      ReasonReact.UpdateWithSideEffects(
        {...state, showDialog: false},
        (self => self.send(EnableSku)),
      )
    | EnableSku => ReasonReact.Update({...state, skuEnabled: true})
    | DisableSku => ReasonReact.Update({...state, skuEnabled: false})
    },
  initialState: () => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let customerName =
      switch (Util.QueryParam.get("customerName", queryString)) {
      | Some(name) => name |> Js.Global.decodeURIComponent
      | None => "order.defaultCustomerName" |> Lang.translate
      };
    {
      closedOrder: false,
      allProducts: [],
      tags: [],
      viewing: Tags,
      order: buildNewOrder(customerName),
      modifying: Nothing,
      allDiscounts: [],
      sku: "",
      showDialog: false,
      skuEnabled: true,
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
      <SearchModal
        label="modal.SearchProductHeader"
        isOpen=self.state.showDialog
        allProducts=self.state.allProducts
        onSelect=(p => self.send(SelectProduct(p)))
        onCancel=(_ => self.send(HideDialog))
      />
      <div className="order-header">
        <OrderActions
          order=self.state.order
          onFinish=(_ => self.send(CloseOrderScreen))
        />
        <div className="order-actions">
          <Button
            local=true
            className="pay-button-card"
            label="order.searchProduct"
            onClick=(_ => self.send(ShowDialog))
          />
        </div>
        <div className="customer-name">
          <EditableText
            mode=TouchToEdit
            text=self.state.order.customerName
            onChange=(newName => self.send(ChangeCustomerName(newName)))
          />
        </div>
      </div>
      <div className="right-side">
        <SkuSearch
          acceptInput=self.state.skuEnabled
          allProducts=self.state.allProducts
          productFound=(p => self.send(SelectProduct(p)))
        />
        <OrderItems
          closed=self.state.closedOrder
          order=self.state.order
          onChange=(order => self.send(ChangeOrder(order)))
        />
      </div>
      <div className="left-side">
        (
          if (self.state.closedOrder) {
            <ClosedOrderInfo
              order=self.state.order
              paidDateChanged=(newDate => self.send(ChangePaidDate(newDate)))
            />;
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
                    <Button
                      local=true
                      className="back-button-card"
                      onClick=deselectTag
                      label="nav.back"
                    />
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
              <div className="more-actions">
                <Button
                  local=true
                  onClick=(
                    _ =>
                      WebhookEngine.getWebhooks(PrintOrder, Order)
                      |> WebhookEngine.fire(
                           self.state.order |> Order.fromVm |> Order.toJs,
                         )
                      |> ignore
                  )
                  label="order.printOrder"
                />
              </div>
            </div>;
          }
        )
      </div>
    </div>;
  },
};